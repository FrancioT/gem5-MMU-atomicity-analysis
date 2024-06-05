# Copyright (c) 2021 The Regents of the University of California
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""
This example runs a simple linux boot. It uses the 'riscv-disk-img' resource.
It is built with the sources in `src/riscv-fs` in [gem5 resources](
https://github.com/gem5/gem5-resources).

Characteristics
---------------

* Runs exclusively on the RISC-V ISA with the classic caches
* Assumes that the kernel is compiled into the bootloader
* Automatically generates the DTB file
* Will boot but requires a user to login using `m5term` (username: `root`,
  password: `root`)
"""
import m5
from m5.objects import Root

from gem5.components.boards.riscv_board import RiscvBoard
from gem5.components.cachehierarchies.ruby.mesi_two_level_cache_hierarchy import (
    MESITwoLevelCacheHierarchy,
)
from gem5.coherence_protocol import CoherenceProtocol
from gem5.components.memory import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_switchable_processor import (
    SimpleSwitchableProcessor,
)
from gem5.isas import ISA
from gem5.resources.resource import DiskImageResource, CheckpointResource, KernelResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# Run a check to ensure the right version of gem5 is being used.
requires(
    isa_required=ISA.RISCV,
    coherence_protocol_required=CoherenceProtocol.MESI_TWO_LEVEL,
)

# Setup the cache hierarchy.
# For classic, PrivateL1PrivateL2 and NoCache have been tested.
# For Ruby, MESI_Two_Level and MI_example have been tested.
cache_hierarchy = MESITwoLevelCacheHierarchy(
    l1d_size="32kB",
    l1d_assoc=8,
    l1i_size="32kB",
    l1i_assoc=8,
    l2_size="256kB",
    l2_assoc=16,
    num_l2_banks=2,
)
# Setup the system memory.
memory = SingleChannelDDR3_1600()

# Setup a single core Processor.
processor = SimpleSwitchableProcessor(
    starting_core_type = CPUTypes.ATOMIC,
    switch_core_type=CPUTypes.O3,
    isa=ISA.RISCV,
    num_cores=3
)

# Setup the board.
board = RiscvBoard(
    clk_freq="1GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)

# Set the Full System workload.
board.set_kernel_disk_workload(
    kernel=KernelResource("~/Documents/gem5/tests/test-progs/binaries/riscv-bootloader-vmlinux-5.10"),
    #kernel=obtain_resource("riscv-bootloader-vmlinux-5.10","~/Documents/gem5/tests/test-progs/binaries/"),
    disk_image=DiskImageResource("~/Documents/gem5/tests/test-progs/disks/riscv-disk-img"),
    readfile="~/Documents/prova",
    checkpoint = CheckpointResource("~/Documents/m5out/cpt.riscv.282086538000/")
)

simulator = Simulator(board=board)
print("Beginning simulation!")
# Note: This simulation will never stop. You can access the terminal upon boot
# using m5term (`./util/term`): `./m5term localhost <port>`. Note the `<port>`
# value is obtained from the gem5 terminal stdout. Look out for
# "system.platform.terminal: Listening for connections on port <port>".
simulator.run()
