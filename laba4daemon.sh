#!/usr/bin/env bash

PROCESS_FILE_NAME="/var/run/laba4daemon.pid"

case $1 in
start)
  if [[ -e $PROCESS_FILE_NAME ]]; then
    cat $PROCESS_FILE_NAME | xargs -I {} echo "Process already started. PID {}"
    exit 0
  fi

  ./main
;;
stop)
  if [[ ! -e $PROCESS_FILE_NAME ]]; then
      echo "Process is not started"
      exit 0
  fi

  cat $PROCESS_FILE_NAME | xargs -I {} kill {} || exit 1;
  rm $PROCESS_FILE_NAME
;;
force-reload|restart)
  if [[ ! -e $PROCESS_FILE_NAME ]]; then
        echo "Process is not started"
        exit 0
  fi

  cat $PROCESS_FILE_NAME | xargs -I {} kill {} || exit 1;
  ./main
;;
*)
  echo "Usage: arguments {start|stop|restart|force-reload}"
  exit 1
;;
esac