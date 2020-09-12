#/bin/bash
for FILE in test/*;
do echo ${FILE}; ./Halloween.out $FILE;
done
