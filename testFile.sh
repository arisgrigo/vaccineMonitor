#!/bin/bash

line=1
>inputFile.txt
echo "Now building inputFile.txt..."

duplicateLine=$RANDOM
let "duplicateLine %= $3"
if [ $duplicateLine -le 2 ]
then
  let "duplicateLine += 3"
fi

arr=( " ") #array where all ids are kept (to check for duplicates)

while [ $line -le $3 ]
do
  #FLOOR and RANGE are ued to generate random string lengths for first and last name
  FLOOR=1
  RANGE=16

  num=0
  id=0

  IDFLOOR=0
  IDRANGE=9999

  while [ "$id" -le $IDFLOOR ]
  do
    id=$RANDOM
    let "id %= $IDRANGE"
  done

  idcopy=$id
  printf -v idCopyPrint '%04d' "$idcopy"
  #if duplicates are allowed
  if [ $4 == 1 ]
  then
    check=0
    let "check = $line"
    let "check%=20"
    #if duplicates are allowed, then every 20 lines, we make sure that the line is a duplicate (and probably a valid one)
    #if [ $duplicateLine == $line ]     #I used duplicateLine if I wanted to make sure that at least 1 record was duplicate, no matter the line size
    if [ $check == 0 ]
    then
      num=$RANDOM
      lineNum=0
      lineNum=$(wc -l < inputFile.txt)

      let "num %= $lineNum"
      let "num += 1"

      #get random line from the lines we have already made (only the 5 first fields)
      copyLine=$(printf "%s " $(sed  "${num}q;d" inputFile.txt | cut -d ' ' -f 1,2,3,4,5))
      printf '%s' "$copyLine" >> inputFile.txt
      num=$RANDOM

      lineNum=0
      lineNum=$(wc -l < $2)

      let "lineNum += 1"

      let "num %= $lineNum"
      let "num += 1"

      #get random line from virusesFile
      printf "%s " $(sed "${num}q;d" $2) >> inputFile.txt

      num=$RANDOM
      let "num %= 2"

      #50% chance that the citizen has been vaccinated or not
      if [ $num == 1 ]
      then
        printf "YES " >> inputFile.txt
        day=0

        DAYFLOOR=0
        DAYRANGE=31

        while [ "$day" -le $DAYFLOOR ]
        do
          day=$RANDOM
          let "day %= $DAYRANGE"
        done
        printf -v dayPrint '%02d' "$day"
        printf '%s-' "$dayPrint" >> inputFile.txt

        month=0

        MONTHFLOOR=0
        MONTHRANGE=13

        while [ "$month" -le $MONTHFLOOR ]
        do
          month=$RANDOM
          let "month %= $MONTHRANGE"
        done
        printf -v monthPrint '%02d' "$month"
        printf '%s-' "$monthPrint" >> inputFile.txt

        year=0

        YEARFLOOR=1960
        YEARRANGE=2022

        while [ "$year" -le $YEARFLOOR ]
        do
          year=$RANDOM
          let "year %= $YEARRANGE"
        done
        printf -v yearPrint '%04d' "$year"
        printf '%s\n' "$yearPrint" >> inputFile.txt
      else
        printf "NO\n" >> inputFile.txt
      fi
      ((line++))
      ##continue to next line
      continue;
    fi
    #check to see if the random id has been given before
    if [[ " ${arr[@]} " =~ " ${id}" ]];
    then
      lineString=""
      lineString=$(awk /$idCopyPrint/ inputFile.txt)
      #if the id exists, get the line in the file and check if the string is NULL
      if [ ! -z "$lineString" ]
      then
        #if the string is not NULL then there's a ~20% chance that the new line is valid (same 5 fields as other line, while virus field differs)
        #so that means that there's an ~80% chance that the new line will be invalid (inconsistent, since new line will have same id field as another line, but the rest will differ)
        num=$RANDOM
        let "num %= 5"
        if [ $num == 0 ]
        then
          #get the first 5 fields of the previous line
          newLine=$(echo -e $lineString | cut -d ' ' -f 1,2,3,4,5)
          printf '%s ' "$newLine" >> inputFile.txt
          num=$RANDOM

          lineNum=0
          lineNum=$(wc -l < $2)

          let "lineNum += 1"

          let "num %= $lineNum"
          let "num += 1"

          #get random virus from virusesFile
          printf "%s " $(sed "${num}q;d" $2) >> inputFile.txt

          num=$RANDOM
          let "num %= 2"

          if [ $num == 1 ]
          then
            printf "YES " >> inputFile.txt
            day=0

            DAYFLOOR=0
            DAYRANGE=31

            while [ "$day" -le $DAYFLOOR ]
            do
              day=$RANDOM
              let "day %= $DAYRANGE"
            done
            printf -v dayPrint '%02d' "$day"
            printf '%s-' "$dayPrint" >> inputFile.txt

            month=0

            MONTHFLOOR=0
            MONTHRANGE=13

            while [ "$month" -le $MONTHFLOOR ]
            do
              month=$RANDOM
              let "month %= $MONTHRANGE"
            done
            printf -v monthPrint '%02d' "$month"
            printf '%s-' "$monthPrint" >> inputFile.txt

            year=0

            YEARFLOOR=1960
            YEARRANGE=2022

            while [ "$year" -le $YEARFLOOR ]
            do
              year=$RANDOM
              let "year %= $YEARRANGE"
            done
            printf -v yearPrint '%04d' "$year"
            printf '%s\n' "$yearPrint" >> inputFile.txt
          else
            printf "NO\n" >> inputFile.txt
          fi
          ((line++))
          continue;
        fi
    fi
    else
      #if the id does not exist in the array, add it
      arr+=( "$id")
    fi
  fi
  #if duplicates are not allowed check the id array to see if the id has been given before
  #if it has, skip this loop, else, add it to the array
  if [ $4 == 0 ]
  then
    if [[ " ${arr[@]} " =~ " ${id}" ]];
    then
      continue
    else
      arr+=( "$id")
    fi
  fi

  printf -v idPrint '%04d' "$id"

  printf '%s ' "$idPrint" >> inputFile.txt

  while [ "$num" -le $FLOOR ]
  do
    num=$RANDOM
    let "num %= $RANGE"
  done

  chars=abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ
  for i in $( seq 0 $num) ; do
    echo -n "${chars:RANDOM%${#chars}:1}" >> inputFile.txt
  done

  echo -n ' ' >> inputFile.txt

  num=0
  while [ "$num" -le $FLOOR ]
  do
    num=$RANDOM
    let "num %= $RANGE"
  done

  for i in $( seq 0 $num) ; do
    echo -n "${chars:RANDOM%${#chars}:1}" >> inputFile.txt
  done

  echo -n ' ' >> inputFile.txt

  num=$RANDOM
  lineNum=0
  lineNum=$(wc -l < $1)

  let "lineNum += 1"

  let "num %= $lineNum"
  let "num += 1"

  printf "%s " $(sed  "${num}q;d" $1) >> inputFile.txt

  age=0
  AGEFLOOR=0
  AGERANGE=120

  while [ "$age" -le $IDFLOOR ]
  do
    age=$RANDOM
    let "age %= $AGERANGE"
  done

  printf -v agePrint '%d' "$age"

  printf '%s ' "$agePrint" >> inputFile.txt

  num=$RANDOM

  lineNum=0
  lineNum=$(wc -l < $2)


  let "lineNum += 1"

  let "num %= $lineNum"
  let "num += 1"

  printf "%s " $(sed "${num}q;d" $2) >> inputFile.txt

  num=$RANDOM
  let "num %= 2"

  if [ $num == 1 ]
  then
    printf "YES " >> inputFile.txt
    day=0

    DAYFLOOR=0
    DAYRANGE=31

    while [ "$day" -le $DAYFLOOR ]
    do
      day=$RANDOM
      let "day %= $DAYRANGE"
    done
    printf -v dayPrint '%02d' "$day"
    printf '%s-' "$dayPrint" >> inputFile.txt

    month=0

    MONTHFLOOR=0
    MONTHRANGE=13

    while [ "$month" -le $MONTHFLOOR ]
    do
      month=$RANDOM
      let "month %= $MONTHRANGE"
    done
    printf -v monthPrint '%02d' "$month"
    printf '%s-' "$monthPrint" >> inputFile.txt

    year=0

    YEARFLOOR=1960
    YEARRANGE=2022

    while [ "$year" -le $YEARFLOOR ]
    do
      year=$RANDOM
      let "year %= $YEARRANGE"
    done
    printf -v yearPrint '%04d' "$year"
    printf '%s\n' "$yearPrint" >> inputFile.txt
  else
    printf "NO\n" >> inputFile.txt
  fi
  ((line++))
done
echo "inputFile.txt has been built"