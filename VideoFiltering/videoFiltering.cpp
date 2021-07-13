#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const uint32_t BIG_ENDIAN_ID    = 0x4d6d6d4d;
const uint32_t LITTLE_ENDIAN_ID = 0x49696949;
const uint32_t FRAME_ID         = 0x46727246;
const uint32_t VIDEO_DATA_ID    = 0x56696956;
const uint32_t AUDIO_LIST_ID    = 0x416c6c41;
const uint32_t AUDIO_DATA_ID    = 0x41757541;
#endif /* __PROGTEST__ */

class ErrorOrder{};
class ErrorFile{};
class ErrorMisMacth{};

uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

istream & readPay(istream & ifile, uint32_t & read_numb, uint32_t len){
    if(len <= 4){
        ifile.read((char*)& read_numb, len);
        return ifile;
    }
    ifile.read((char*)& read_numb, 4);
    return readPay(ifile, read_numb, len - 4);
}

ostream & readPay(istream & ifile, ostream & ofile, uint32_t & read_numb, uint32_t len){
    if(len <= 4){
        ifile.read((char*)& read_numb, len);
        ofile.write((char*)&  read_numb, len);
        return ofile;
    }
    ifile.read((char*)& read_numb, 4);
    ofile.write((char*)&  read_numb, 4);
    return readPay(ifile, ofile, read_numb, len - 4);
}

uint32_t & read4numb(istream & ifile, uint32_t & read_numb){
    ifile.read((char*)& read_numb, 4);
    return read_numb;
}

uint32_t & read4numb(istream & ifile, uint32_t & read_numb, uint32_t & cnt){
    ifile.read((char*)& read_numb, 4);
    cnt += 4;
    return read_numb;
}

ostream & write4numb(ostream & ofile, uint32_t & read_numb){
    ofile.write((char*)& read_numb, 4);
    return ofile;
}

ostream & setVal(ostream & ofile, int & pos_frames, int & pos_alist, int & pos_lang,
                 uint32_t &  cnt_frames, uint32_t &  cnt_aList, uint32_t & lang_numb, int var){
    auto tmp = ofile.tellp();
    if(var == 2){
        cnt_frames = swap_uint32(cnt_frames);
        cnt_aList = swap_uint32(cnt_aList);
        lang_numb = swap_uint32(lang_numb);
    }
    ofile.seekp(pos_lang);
    write4numb(ofile, lang_numb);
    ofile.seekp(pos_alist);
    write4numb(ofile, cnt_aList);
    ofile.seekp(pos_frames);
    write4numb(ofile, cnt_frames);
    ofile.seekp(tmp);
    return ofile;
}

ostream & readAudioData(ostream & ofile, istream & ifile, uint32_t & read_numb,
                        uint32_t & cnt_aList, uint32_t & lang_numb, const char * lang_search, int var){
    int pos_id = ifile.tellg();// fix position before read id
    read_numb = read4numb(ifile, read_numb);//read id
    if(read_numb != AUDIO_DATA_ID){
        throw ErrorOrder();
    }
    char lang [2];
    ifile.read(lang, 2);// read language
    if(lang[0] == lang_search[0] && lang[1] == lang_search[1]){
        char* tmp_lang = new char[2];
        ifile.seekg(pos_id);// return to the id position
        read_numb = read4numb(ifile, read_numb, cnt_aList);//read id
        write4numb(ofile,read_numb);
        ifile.read(tmp_lang, 2);// read lang
        ofile.write(tmp_lang, 2);
        cnt_aList += 2;
        read_numb = read4numb(ifile, read_numb, cnt_aList);// read length of payload
        write4numb(ofile,read_numb);//write length of payload
        if(var == 2){
            read_numb = swap_uint32(read_numb);
        }
        uint32_t len_pay = read_numb;
        cnt_aList += len_pay;
        readPay(ifile, ofile, read_numb, len_pay);
        read_numb = read4numb(ifile, read_numb, cnt_aList);//read crc
        write4numb(ofile, read_numb);
        delete []tmp_lang;
    }
    else{
        lang_numb -= 0x01;
        char* tmp_lang = new char[2];
        ifile.seekg(pos_id);// return to the id position
        read_numb = read4numb(ifile, read_numb);//read id
        ifile.read(tmp_lang, 2);// read lang
        read_numb = read4numb(ifile, read_numb);//read len pay load;
        if(var == 2){
            read_numb = swap_uint32(read_numb);
        }
        uint32_t len_pay = read_numb;
        readPay(ifile, read_numb, len_pay);
        read_numb = read4numb(ifile, read_numb);//read crc
        delete []tmp_lang;
    }
    return ofile;
}

ostream & readAudioList(ostream & ofile, istream & ifile, uint32_t & read_numb, uint32_t & cnt_frames,
                        uint32_t & cnt_aList, uint32_t & lang_numb, int & pos_aList, int & pos_lang,
                        const char * lang_search, int var){
    read_numb = read4numb(ifile, read_numb, cnt_frames);//read audioList id
    if(read_numb != AUDIO_LIST_ID){
        throw ErrorOrder();
    }
    write4numb(ofile, read_numb);
    pos_lang = ofile.tellp();// fix the pos of lang numb
    read_numb = read4numb(ifile, read_numb, cnt_frames);//read lang amount
    write4numb(ofile, read_numb);
    if(var == 2){
        read_numb = swap_uint32(read_numb);
    }
    lang_numb = read_numb;
    pos_aList = ofile.tellp();// fix len of payload
    read_numb = read4numb(ifile, read_numb, cnt_frames);//read len of payload
    write4numb(ofile, read_numb);
    if(var == 2){
        read_numb = swap_uint32(read_numb);
    }
    auto len = read_numb;
    auto pos_tmp = ifile.tellg();
    for(auto i = lang_numb; i != 0; i--){
        readAudioData(ofile, ifile, read_numb,
                      cnt_aList, lang_numb, lang_search, var);
    }
    if((-pos_tmp) + ifile.tellg() != len){
        throw ErrorMisMacth();
    }
    read_numb = read4numb(ifile, read_numb, cnt_frames);//read crc
    write4numb(ofile, read_numb);
    cnt_frames += cnt_aList;
    return ofile;
}

ostream & readVideo(ostream & ofile, istream & ifile, uint32_t & read_numb,
                    uint32_t & cnt_frames, int var){
    read_numb = read4numb(ifile, read_numb, cnt_frames);// read VIDEO id
    if(read_numb != VIDEO_DATA_ID){
        throw ErrorOrder();
    }
    write4numb(ofile, read_numb);
    read_numb = read4numb(ifile, read_numb, cnt_frames);//read len of payload
    write4numb(ofile, read_numb);
    if(var == 2){
        read_numb = swap_uint32(read_numb);
    }
    uint32_t len_video = read_numb;
    cnt_frames += len_video;
    readPay(ifile, ofile, read_numb, len_video);
    read_numb = read4numb(ifile, read_numb, cnt_frames);// read crc
    write4numb(ofile, read_numb);
    return ofile;
}

ostream & readFrames(ostream & ofile, istream & ifile, uint32_t & read_numb, uint32_t & cnt_global,
                     uint32_t & cnt_frames, int & pos_frames, int & check_frames, uint32_t & len_frames, int var){
    read_numb = read4numb(ifile, read_numb, cnt_global);// read FRAME id
    if(read_numb != FRAME_ID){
        throw ErrorOrder();
    }
    write4numb(ofile, read_numb);
    pos_frames = ofile.tellp();// fix len of payload
    read_numb = read4numb(ifile, read_numb, cnt_global);//read len payload
    write4numb(ofile, read_numb);
    if(var == 2){
        read_numb = swap_uint32(read_numb);
    }
    len_frames = read_numb;
    check_frames = ifile.tellg();
    return ofile;
}


ostream & readLittleInd(ostream & ofile, istream & ifile, uint32_t & read_numb, const char * lang_search, int var){
    read_numb = read4numb(ifile, read_numb);//read number of frames
    write4numb(ofile, read_numb);
    if(var == 2){
        read_numb = swap_uint32(read_numb);
    }
    uint32_t frames_m = read_numb;
    int pos_len_glob = ofile.tellp();//fixed the position of payload
    read_numb = read4numb(ifile, read_numb);//read len of payload
    write4numb(ofile, read_numb);
    if(var == 2){
        read_numb = swap_uint32(read_numb);
    }
    uint32_t len_pay = read_numb;
    int tmp_pos = ifile.tellg();
    uint32_t cnt_global = 0;
    for(auto i = frames_m; i != 0; i--){
        uint32_t len_frames = 0;
        uint32_t cnt_frames = 0, cnt_aList = 0, lang_numb = 0;// counter for global pay load len
        int pos_frames = 0, pos_alist = 0, pos_lang = 0;
        int check_frames = 0;
        readFrames(ofile, ifile, read_numb, cnt_global, cnt_frames, pos_frames, check_frames,
                   len_frames, var);//read frames
        readVideo(ofile, ifile, read_numb, cnt_frames, var);
        readAudioList(ofile, ifile, read_numb, cnt_frames, cnt_aList, lang_numb, pos_alist,
                      pos_lang, lang_search, var);
        if(((-check_frames) + ifile.tellg()) != len_frames){
            throw ErrorMisMacth();
        }
        read4numb(ifile, read_numb, cnt_global);
        write4numb(ofile,read_numb);
        cnt_global += cnt_frames;
        setVal(ofile, pos_frames, pos_alist, pos_lang,
           cnt_frames, cnt_aList, lang_numb, var);
    }
    if((-tmp_pos + ifile.tellg()) != len_pay){
        throw ErrorMisMacth();
    }
    read_numb = read4numb(ifile, read_numb);// read crc
    write4numb(ofile,read_numb);
    auto tmp = ofile.tellp();
    ofile.seekp(pos_len_glob);
    if(var == 2){
        cnt_global = swap_uint32(cnt_global);
    }
    write4numb(ofile,cnt_global);
    ofile.seekp(tmp);
    return ofile;
}


bool filterFile          ( const char      * srcFileName,
                           const char      * dstFileName,
                           const char      * lang )
{
    try {
        uint32_t read_numb = 0;
        ifstream ifile(srcFileName, ios::in | ios::binary);
        ofstream ofile(dstFileName, ios::out | ios::binary);
        if(!ifile.good() || !ofile.good()){
            throw ErrorFile();
        }
        read_numb = read4numb(ifile, read_numb);
        write4numb(ofile, read_numb);
        int var = 0;
        if (read_numb == LITTLE_ENDIAN_ID) {
            var = 1;
        }
        else if ( read_numb == BIG_ENDIAN_ID){
            var = 2;
        }
        else{
            throw ErrorMisMacth();
        }
        readLittleInd(ofile, ifile, read_numb, lang, var);
        if(!ifile.good() || !ofile.good()){
            throw ErrorFile();
        }
        if(ifile.peek() != EOF){
            throw ErrorFile();
        }
        return true;
    }
    catch( ErrorOrder ex ){
        return false;
    }
    catch (ErrorFile ex) {
        return false;
    }
    catch(ErrorMisMacth ex){
        return false;
    }
}

#ifndef __PROGTEST__
int main(){
    assert(filterFile("in_4018471.bin",  "test.out" ,"cs") == false);
    return 0;
}
#endif /* __PROGTEST__ */
