package picarus_takeout

/*
#cgo LDFLAGS: -lpicarusto -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_objdetect -lopencv_features2d -lmsgpack -lfftw3f
#include <stdlib.h>
#include "../picarus_takeout/picarus_takeout_wrap.hpp"
*/
import "C"
import "unsafe"


func ModelChainProcessBinary(model string, input string) string {
	modelp := C.CString(model)
	defer C.free(unsafe.Pointer(modelp))
	inputp := C.CString(input)
	defer C.free(unsafe.Pointer(inputp))
	var sizeOut C.int
	out := unsafe.Pointer(C.picarus_takeout_model_chain_process_binary(modelp, C.int(len(model)), inputp, C.int(len(input)), &sizeOut))
	outS := string(C.GoBytes(out, sizeOut))
	C.picarus_takeout_delete_output(out)
	return outS
}