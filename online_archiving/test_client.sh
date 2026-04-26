#!/bin/bash

SAMPLE_ARCH=file.ar
PORT=3000

function recv() {
    TMPDIR=$$
    mkdir -p $TMPDIR
    cd $TMPDIR || { echo "Errore: impossibile entrare nella directory recv"; return 1; }
    AR=$$.ar
    # 2. Invoca nc per ricevere l'archivio
    # Il comando rimane in attesa finché il client non chiude la connessione
    nc -l -p $PORT > $AR
    if [ ! -s $AR ]; then
        echo "Errore: file.ar è vuoto o non ricevuto correttamente."
    fi
    diff ../$SAMPLE_ARCH $AR
    if [ $? -eq 0 ]
    then
        echo "Archive structure OK"
    else
        echo "Archive structure is not correct"
    fi
    echo -e "*** Elenco file nell'archivio ---"
    ar t $AR
    ar x $AR
    for F in $(ar t $AR)
    do
        HASHLOC="$(md5sum $F | cut -f1 -d' ')"
        HASHREM="$(md5sum ../$F | cut -f1 -d' ')"
        if [ "$HASHLOC" == "$HASHREM" ]
        then
            echo "File $F matches source with hash $HASHLOC"
        else
            echo "File $F: expecing hash $HASHREM, got $HASHLOC"
        fi
    done
    cd ..
    rm -rf $TMPDIR
}


# cleanup
pkill nc 2>/dev/null

if [ $# -ge 1 ]
then 
    CLIENT=$1
else
    CLIENT=client.py
fi
recv &
sleep 2
ar rc $SAMPLE_ARCH prayer.txt fear.txt
if [[ "$CLIENT" == *.py ]]
then
    echo "executing python script $CLIENT"
    python3 $CLIENT localhost 3000 prayer.txt fear.txt 
else 
    echo "executing executable $CLIENT"
    $CLIENT localhost 3000 prayer.txt fear.txt 
fi 
sleep 5
rm $SAMPLE_ARCH

