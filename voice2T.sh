#!/bin/bash
FOLDER=$1

cd $FOLDER

# ls *.wav > targetList
for i in *.wav
do
  echo "${i%.*}.wav ${i%.*}.mfcc" >> targetList
done
HCopy -C /home/wenzheng/VoiceCal/calculator/test/config -S targetList
ls *.mfcc > list.txt
HVite -A -T 1 -S list.txt -H /home/wenzheng/VoiceCal/calculator/model/hmm16/hmm16.mmf -i recog.mlf -w /home/wenzheng/VoiceCal/calculator/def/net.slf /home/wenzheng/VoiceCal/calculator/def/dict /home/wenzheng/VoiceCal/calculator/hmmlist.txt
python ../transOut.py recog.mlf input.txt
UtoE input.txt out.json
python ../createTextForFest.py out.json output.txt
text2wave -o result.wav output.txt
