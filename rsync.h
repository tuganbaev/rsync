#pragma once

#include <vector>
#include <string>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct Frame {
    int32_t msg_id;
    bool last;
    std::vector<char> body;
};

class Connection {
public:
    virtual ~Connection() {}

    virtual void WriteFrame(Frame* frame) = 0;
    virtual void ReadFrame(Frame* frame) = 0;
};

enum class MsgId {
    GETLIST = 1,
    FILELIST = 2,
    REMOVE = 3,
    FILE = 4,
    FILE_PART = 5,
    OK = 6
};

struct FileList {
    std::vector<std::string> files;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & files;
    }
};

class Protocol {
public:
    void SendGetList();
    void SendFileList(const FileList& list);

    MsgId RecvMsg();
    FileList* GetFileList();
};
