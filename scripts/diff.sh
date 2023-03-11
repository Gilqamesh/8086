#!bin/sh

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

cd ../asm

for ASM_FILE in *.asm
do
    CUR_FILE_BASE="${ASM_FILE%.*}"
    # run nasm over .asm file
    nasm -o $CUR_FILE_BASE.original_bin $ASM_FILE
    
    # run mynasm over the binary generated from last step
    ../src/mynasm $CUR_FILE_BASE.original_bin > $CUR_FILE_BASE.own

    # run nasm over disassembled files from last step
    nasm -o $CUR_FILE_BASE.own_bin $CUR_FILE_BASE.own

    # test to see if there is a difference between the two binaries generated from nasm and mynasm
    diff $CUR_FILE_BASE.original_bin $CUR_FILE_BASE.own_bin > ../diff_results/$CUR_FILE_BASE.diff
    DIFF_EXIT_CODE=$?
    if [ "$DIFF_EXIT_CODE" -eq 0 ]; then
        echo "${GREEN}success: $CUR_FILE_BASE ${ENDCOLOR}"
        echo The two files are identical > ../diff_results/$CUR_FILE_BASE.diff
    else
        echo "${RED}failure: $CUR_FILE_BASE ${ENDCOLOR}"
    fi
done

# rm *.original_bin
rm *.own_bin
rm *.own

cd ../src
