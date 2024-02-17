#!/bin/bash

REMOTE_USER="gehnbrig"
REMOTE_HOST="192.168.1.6"
REMOTE_DIR="/usr/local/bin"
export SSH_KEY=/home/gehnbrig/.ssh/id_rsa

scp -i $SSH_KEY -v -P 22 cat/s21_cat $REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR
scp -i $SSH_KEY -v -P 22 grep/s21_grep $REMOTE_USER@$REMOTE_HOST:$REMOTE_DIR
