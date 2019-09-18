package main

import (
	"encoding/json"
	"io"
	"log"
	"net"
	"net/http"
)

type Version struct {
	Controller string `json:"controller"`
	Api        string `json:"api"`
}

func apiVersion(w http.ResponseWriter, req *http.Request) {
	version := Version{Controller: "0.1", Api: "1.0"}
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.WriteHeader(http.StatusOK)
	json.NewEncoder(w).Encode(version)
}

func reader(r io.Reader) {
	buf := make([]byte, 1024)
	for {
		n, err := r.Read(buf[:])
		if err != nil {
			return
		}
		println("Client got:", string(buf[0:n]))
	}
}

func main() {
	c, err := net.Dial("unix", "/tmp/drd.sock")
	if err != nil {
		//panic(err)
	} else {
		defer c.Close()
	}

	//go reader(c)

	http.HandleFunc("/version", apiVersion)

	log.Println("Starting API server...")
	log.Fatal(http.ListenAndServe(":4482", nil))
	// for {
	//   _, err := c.Write([]byte("hi"))
	//   if err != nil {
	//     log.Fatal("write error:", err)
	//     break
	//   }
	//   time.Sleep(1e9)
	//}
}
