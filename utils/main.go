package main

import "fmt"
//import "picarus"
import "os"
import "io"
import picarusto "github.com/bwhite/picarus_takeout/go"

func readFile(filename string) (string, error) {
	fi, err := os.Open(filename)
	if err != nil {
		fmt.Println("Couldn't open file: " + filename)
		return "", err
	}
	defer func() {
		if err := fi.Close(); err != nil {
			fmt.Println("Couldn't close file:" + filename)
		}
	}()
	buf := make([]byte, 1024)
	data := ""
	for {
		// read a chunk
		n, err := fi.Read(buf)
		if err != nil && err != io.EOF {
			fmt.Println("Couldn't read file: " + filename)
			return "", err
		}
		if n == 0 { break }
		data = data + string(buf[:n])
	}
	return data, nil
}

func writeFile(filename string, data string) {
	fo, err := os.Create(filename)
	if err != nil { fmt.Println("Couldn't create file") }
	defer func() {
		if err := fo.Close(); err != nil {
			fmt.Println("Couldn't close file")
		}
	}()
	if _, err := fo.Write([]byte(data)); err != nil {
		fmt.Println("Couldn't write file")
	}
}

func main() {
	model, err := readFile("model.msgpack")
	if err != nil {
		fmt.Println("Couldn't read file")
	}
	input, err := readFile("input.jpg")
	if err != nil {
		fmt.Println("Couldn't read file")
	}
	writeFile("output.msgpack", picarusto.ModelChainProcessBinary(model, input))
}