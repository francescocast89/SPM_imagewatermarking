#!/bin/bash
#cd /home/spm18-castiglione/Project/
now=$(date +"%d-%m-%Y_%H:%M")
sh seq_test.sh | tee -a "logs/output_project"$now".txt"
sh par_test.sh | tee -a "logs/output_project"$now".txt"
sh ff_test.sh | tee -a "logs/output_project"$now".txt"