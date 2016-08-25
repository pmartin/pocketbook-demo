#!/bin/sh

LOCAL_PATH=$1
REMOTE_APP_NAME=$2
REMOTE_IP=$3
REMOTE_PORT=10003

echo "Sending '$LOCAL_PATH' to '$REMOTE_IP:$REMOTE_PORT/$REMOTE_APP_NAME'"

echo "Sending application name..."
echo "$REMOTE_APP_NAME" | nc "$REMOTE_IP" 10003

sleep 1   # The ereader needs a bit of time to re-launch 'nc'
echo "Sending application content..."
cat "$LOCAL_PATH" | nc "$REMOTE_IP" 10003
