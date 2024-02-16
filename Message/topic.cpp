#include "Message.hpp"

void Message::topicExecute(Server &server, Client &client)
{
    // 파라미터 있다면,
        // 파라미터 개수 확인
            // 채널명 
                // 해당 채널 있는지 확인 => 있으면 토픽 응답, 없으면 no such 채널
            // 채널명 + 인자
                // 해당 채널 있는지 확인 => 없으면 no such 채널
                    // 인자 확인 후 
                        // 채널 op인지 확인 후, 토픽 세팅 해주기

    // 파라미터 없다면,
        // needmoreparameter 에러
}
