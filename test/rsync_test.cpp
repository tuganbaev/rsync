#include "rsync.h"

#include <thread>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <gtest/gtest.h>

class FakeConnection : public Connection {
public:
    Frame frame;
    virtual void WriteFrame(Frame* f) { frame = *f; }
    virtual void ReadFrame(Frame* f) { *f = frame; }
};

TEST(FakeConnection, RemembersFrame) {
    FakeConnection c;
    Frame f;
    f.msg_id = MsgId::GETLIST;
    c.WriteFrame(&f);
    c.ReadFrame(&f);
}

TEST(Protocol, GetList) {
    FakeConnection c;
    Protocol p(&c);

    p.SendGetList();
    ASSERT_EQ(MsgId::GETLIST, c.frame.msg_id);

    ASSERT_EQ(MsgId::GETLIST, p.RecvMsg());
}

TEST(Protocol, FileList) {
    FakeConnection c;
    Protocol p(&c);

    FileList l;
    l.files = { "a.txt", "b.txt", "c.txt" };

    p.SendFileList(l);
    ASSERT_EQ(MsgId::FILELIST, c.frame.msg_id);

    ASSERT_EQ(MsgId::FILELIST, p.RecvMsg());
    ASSERT_EQ(l.files, p.GetFileList().files);
}

TEST(SocketConnection, ReadWrite) {
    int socks[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, socks));

    SocketConnection sender(socks[0]), receiver(socks[1]);

    Frame f1, f2;
    f1.msg_id = MsgId::OK;
    f1.body = "body";

    sender.WriteFrame(&f1);
    receiver.ReadFrame(&f2);

    ASSERT_EQ(f1.msg_id, f2.msg_id);
    ASSERT_EQ(f1.body, f2.body);
}

TEST(Thread, Example) {
    std::thread t1([&] {
        std::cout << "1" << std::endl;
        usleep(20000);
        std::cout << "2" << std::endl;
    });

    std::thread t2([&] {
        usleep(10000);
        std::cout << "3" << std::endl;
        usleep(20000);
        std::cout << "4" << std::endl;
    });
    t1.join();
    t2.join();
}
