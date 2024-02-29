#pragma once
#include <string>

#include "rapidjson/document.h" 
#include "rapidjson/filewritestream.h"  // FileWriteStream
#include "rapidjson/filereadstream.h" 
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/encodedstream.h"    // EncodedOutputStream


namespace json {
    using namespace rapidjson;

    

    //Should not use UTF32
    template <typename UTFI = UTF16<>, typename UTFO = UTF16LE<>>
    int WriteInUTF(GenericDocument<UTFI>& d, const char* file) {
        char* writeBuffer;// 256 for UTF8, 512 fot UTF16
        int buffSize = 0;

        if (std::is_same<UTFI, UTF8<>>::value) buffSize = 256;
        else buffSize = 512;
        writeBuffer = new char[buffSize];
        
        //use C-style API to work with files, for fstream API needs to be wrapped and this is excessive pain in ass 
        FILE* fp = fopen(file, "wb"); // non-Windows use "w"
        if (fp == NULL) return 1;

        FileWriteStream bos(fp, writeBuffer, buffSize);

        typedef EncodedOutputStream<UTFO, FileWriteStream> OutputStream;
        OutputStream eos(bos, true);   // Write BOM

        PrettyWriter<OutputStream, UTFI, UTFO> writer(eos);
        d.Accept(writer);   // This generates UTF16-LE file from UTF-16 in memory
        

        fclose(fp);
        delete[] writeBuffer;

        return 0;
    }

    template <typename UTFI = UTF16<>, typename UTFO = UTF16LE<>>
    int ReadFromUTF(GenericDocument<UTFI>& d, const char* file) {
        char* writeBuffer;// 256 for UTF8, 512 fot UTF16
        int buffSize = 0;

        if (std::is_same<UTFI, UTF8<>>::value) buffSize = 256;
        else buffSize = 512;
        writeBuffer = new char[buffSize];

        FILE* fp = fopen(file, "rb"); // non-Windows use "r"
        if (fp == NULL) return 1;

        char readBuffer[256];
        FileReadStream bis(fp, readBuffer, sizeof(readBuffer));

        EncodedInputStream<UTF16LE<>, FileReadStream> eis(bis);

        d.ParseStream<0, UTFO>(eis);  // Parses UTF-16LE file into UTF-16 in memory
        if (d.HasParseError()) return 1;

        fclose(fp);
        delete[] writeBuffer;

        return 0;
    }

}