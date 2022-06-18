package main

/*
#ifndef ADDON_H_
#define ADDON_H_
typedef void (*gocallback)(const char *log, void *ptr);
#endif

static void runFunc(gocallback f, const char *msg, void *ptr) {
	f(msg, ptr);
}
*/
import "C"

import (
	"fmt"
	"time"
	"unsafe"
	"strings"
)

type Callback func(msg string)

//export AwesomeJob
func AwesomeJob(second int, cb unsafe.Pointer, cbData unsafe.Pointer) {
	var callback Callback
	if cb == nil {
		callback = func(msg string) {
			fmt.Print(msg)
		}
	} else {
		callback = func(msg string) {
			f := C.gocallback(cb)
			C.runFunc(f, C.CString(strings.TrimSpace(msg)), cbData)
		}
	}

	startTime := time.Now()

	callback(fmt.Sprintf("AwesomeJob started"))

	// High CPU usage
	time.Sleep(time.Duration(second) * time.Second)

	endTime := time.Now()
	callback(fmt.Sprintf("AwesomeJob ended! %s", endTime.Sub(startTime)))
}

func main() {
	AwesomeJob(5, nil, nil)
}