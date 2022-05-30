# HERA
Implementation of HERA algorithm for https://www.fer.unizg.hr/predmet/bio1

Algorithm description:https://www.biorxiv.org/content/10.1101/345983v1.full.pdf

Github repo: https://github.com/Ficiao/HERA

### How to run the program (Linux)
Note: for Windows download the source code, compile it and run with arguments specified below.

Download the BIOINFO-HERA zip from this link -> https://www.dropbox.com/s/ml92mhvskv2k347/BIOINFO-HERA.zip?dl=0 and unpack it to the desired location.


Open the terminal and position it to the directory which has the HERA executable.
Make sure you have all the necessary files to run the program. 

Input parameters are as follows:
- 1 - PAF file with reads overlaps, including reversible complements
- 2 - PAF file with contigs and reads overlaps, including reads reversible complements
- 3 - Number of read nodes
- 4 - Number of contigs
- 5 - FASTA file for reads genome sequences including reversible complements
- 6 - FASTA file for contig genome sequences
- 7 - Name of the file in which the result will be written

Example how to run the program with Ecoli test files:

`./HERA overlap_reads_rc_ava_pb.paf overlaps_reads_rc_contigs.paf 9284 3 ecoli_test_reads_with_complements.fasta ecoli_test_contigs.fasta result`

### Testing the output
Use the Gepard tool (https://cube.univie.ac.at/gepard) and compare the result file with the test reference.


