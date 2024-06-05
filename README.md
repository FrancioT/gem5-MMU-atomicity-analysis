# gem5-MMU-atomicity-analysis
In this repo there are all the tests, traces and scripts used in order to analyze the implementation of the: 
* atomic instructions
* synchronization methods (like load reserve, store conditional)
* the fences and weaker ordering instructions like acquire release
<br />
<br />

The repository structure is defined as follows:
* the "C_scripts" folder contains all the codes used to test the MMU and the synchronization techniques for multi-threading.
* the "Guides_and_presentations" folder contains the gem5 simulation guides, the presentations and the sequence diagrams used during the analysis
* the "gem5_materials" folder contains: the binaries for the kernel and bootloader, the disk images, the build_ops files that must be added in gem5 to build an arm or riscv architecture, with cache consistency model using ruby, and the python scripts written in order to launch the full system simulation.
* the "test_outputs" folder contains all the tests traces obtained with the relative scripts and assembly transletions, the dot files describing the architecture created with the full system simulations and the checkpoints
