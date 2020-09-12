#/bin/bash
for FILE in test/*;
do echo ${FILE}; ./MaxCandy.out $FILE;
done
