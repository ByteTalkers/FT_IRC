#include "Message.hpp"

void Message::topicExecute(Server &server, Client &client)
{
    // 파라미터 있다면,
    std::size_t params_count;
    params_count = this->m_params.size();
    if (params_count > 0)
    {
        std::string channel_name;
        channel_name = this->m_params[0];
        // 해당 채널 존재하는 지 확인
        if (server.channelExist(channel_name) == true) 
        {
            // 파라미터 채널명만 들어온 경우 => 토픽 확인
            Channel *channel;
            channel = server.getChannels().at(channel_name);
            if (params_count == 1) 
            {
                // 토픽 존재여부 확인
                if (channel->getSetTopic() == true)
                {
                    Response::rplTopic_332(server.getName(), client.getNick(), channel_name, channel->getTopic());
                }
                else
                {
                    Response::rplNoTopic_331(server.getName(), client.getNick(), channel_name);
                }
            }
            // 채널명 + 인자인 경우 => 토픽 세팅
            else
            {
                // 채널의 토픽 설정 운영자 전용인지 아닌지 확인
                // 채널 op인지 확인
                if (channel->checkOp(&client) == true)
                {
                    channel->setTopic(this->m_params[1]);
                }
                else
                {
                    Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name);
                }
            }
        }
        // 없으면 no such 채널
        else
        {
            Response::errNoSuchChannel_403(server.getName(), client.getNick(), channel_name);
        }
    }
    // 파라미터 없다면, needmoreparameter 에러
    else
    {
        Response::errNeedMoreParams_461(server.getName(), client.getNick(), this->m_command);
    }
}
