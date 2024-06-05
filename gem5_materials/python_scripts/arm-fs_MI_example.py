# Copyright (c) 2022 The Regents of the University of California
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
This script further shows an example of booting an ARM based full system Ubuntu
disk image. This simulation boots the disk image using 2 TIMING CPU cores. The
simulation ends when the startup is completed successfully (i.e. when an
`m5_exit instruction is reached on successful boot).

Usage
-----

```
scons build/ARM/gem5.opt -j<NUM_CPUS>
./build/ARM/gem5.opt configs/example/gem5_library/arm-ubuntu-run.py
```

"""
import m5
from m5.objects import Root

from m5.objects import (
    ArmDefaultRelease,
    VExpress_GEM5_Foundation,
)

from gem5.components.cachehierarchies.ruby.mi_example_cache_hierarchy import (
    MIExampleCacheHierarchy,
)
from gem5.coherence_protocol import CoherenceProtocol
from gem5.components.boards.arm_board import ArmBoard
from gem5.components.memory import DualChannelDDR4_2400
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_switchable_processor import (
    SimpleSwitchableProcessor,
)
from gem5.isas import ISA
from gem5.resources.resource import DiskImageResource, KernelResource, BootloaderResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# This runs a check to ensure the gem5 binary is compiled for ARM and the
# protocol is CHI.
requires(
    isa_required=ISA.ARM,
    coherence_protocol_required=CoherenceProtocol.MI_EXAMPLE,
)

# Here we setup the parameters of the l1 and l2 caches.
cache_hierarchy = MIExampleCacheHierarchy(
    "32KiB", "8"
)

# Memory: Dual Channel DDR4 2400 DRAM device.

memory = DualChannelDDR4_2400(size="2GB")

# Here we setup the processor. We use a simple TIMING processor. The config
# script was also tested with ATOMIC processor.

processor = SimpleSwitchableProcessor(
    starting_core_type = CPUTypes.ATOMIC,
    switch_core_type=CPUTypes.TIMING,
    isa=ISA.ARM,
    num_cores=1
)

# The ArmBoard requires a `release` to be specified. This adds all the
# extensions or features to the system. We are setting this to Armv8
# (ArmDefaultRelease) in this example config script.
release = ArmDefaultRelease()

# The platform sets up the memory ranges of all the on-chip and off-chip
# devices present on the ARM system.

platform = VExpress_GEM5_Foundation()

# Here we setup the board. The ArmBoard allows for Full-System ARM simulations.

board = ArmBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
    release=release,
    platform=platform,
)

# Here we set a full system workload. The "arm64-ubuntu-20.04-boot" boots
# Ubuntu 20.04.

board.set_kernel_disk_workload(
    kernel=KernelResource("~/Documents/gem5/tests/test-progs/binaries/arm64-linux-kernel-5.4.49"),
    disk_image=DiskImageResource("~/Documents/gem5/tests/test-progs/disks/arm64-ubuntu-20.04-img", root_partition="1"),
    bootloader=BootloaderResource("~/Documents/gem5/tests/test-progs/binaries/arm64-bootloader-foundation"),
)

# We define the system with the aforementioned system defined.

simulator = Simulator(board=board)

# Once the system successfully boots, it encounters an
# `m5_exit instruction encountered`. We stop the simulation then. When the
# simulation has ended you may inspect `m5out/board.terminal` to see
# the stdout.

simulator.run()
