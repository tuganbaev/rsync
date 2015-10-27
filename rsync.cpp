#include "rsync.h"

#include <sstream>

void Protocol::SendGetList() {
    Frame f;
    f.msg_id = MsgId::GETLIST;
    conn_->WriteFrame(&f);
}

void Protocol::SendFileList(const FileList& list) {
    Frame f;
    f.msg_id = MsgId::FILELIST;

    std::stringstream ss;
    boost::archive::text_oarchive archive(ss);
    archive << list;
    f.body = ss.str();

    conn_->WriteFrame(&f);
}

MsgId Protocol::RecvMsg() {
    conn_->ReadFrame(&last_received_);
    return last_received_.msg_id;
}

FileList Protocol::GetFileList() {
    std::stringstream ss;
    ss << last_received_.body;

    FileList list;
    boost::archive::text_iarchive archive(ss);
    archive >> list;

    return list;
}
