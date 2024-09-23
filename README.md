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
# Weak memory order of arm
Under the folder _"C_scripts/acquire_release"_ the problems related to the weak memory order utilized by an ARM processor are studied.

Creating an ad hoc script with and without the use of the Acquire-Release logic it can be osserved a number of memory inconsistency running the script in a simulation with *out of order ARM* processors.
In order to test also in a real case scenario this problem, the script is also executed on a raspberry pi3 b+ and the result can be seen in the subfolder _"C_scripts/acquire_release/tests_on_raspberry"_ with the related image.
