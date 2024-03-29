#!/bin/bash

# Look for the ModifyConfig process
PROCESS_NAME="ModifyConfig"
PID=$(pgrep -f $PROCESS_NAME)

if [ -z "$PID" ]; then
  echo "Process $PROCESS_NAME not found."
else
  echo "Process $PROCESS_NAME found with PID: $PID. Attempting to kill..."
  pkill -f $PROCESS_NAME
  if [ $? -eq 0 ]; then
    echo "Process $PROCESS_NAME has been killed."
  else
    echo "Failed to kill process $PROCESS_NAME."
  fi
fi

