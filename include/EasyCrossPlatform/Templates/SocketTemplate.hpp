#ifndef __EASYCROSSPLATFORM_FILE_SOCKETTEMPLATEHPP_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_SOCKETTEMPLATEHPP_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    #include <EasyCrossPlatform/Templates/DataCarrier.hpp>
    namespace EasyCrossPlatform{
        namespace Templates{
            namespace Sockets{
                class BaseSocket;
                typedef void(*StandardSockErrorCallBack)(int, const std::string&, BaseSocket*);

                class BaseSocket : public EasyCrossPlatform::Templates::VoidPtrDataCarrier{
                    private:
                        StandardSockErrorCallBack m_ErrCB = NULL;
                    protected:
                        void __callErrorCallBack(int ErrNo, const std::string& ErrDescription){
                            if(this->m_ErrCB != NULL){
                                this->m_ErrCB(ErrNo, ErrDescription, this);
                            }
                        }
                    public:
                        virtual std::string getSocketTypeString() = 0;
                        void setOnErrorCallBack(StandardSockErrorCallBack newCallBack){
                            this->m_ErrCB = newCallBack;
                        }
                        StandardSockErrorCallBack getOnErrorCallBack(){
                            return this->m_ErrCB;
                        }
                };

                class SingleConnectionSocket;
                typedef void(*ClientSockMsgCallBack)(const std::vector<byte>&, SingleConnectionSocket*);
                typedef void(*ClientSockDisconnectCallBack)(SingleConnectionSocket*);

                class SingleConnectionSocket : public BaseSocket{
                    private:
                        ClientSockDisconnectCallBack m_DisconnCB = NULL;
                        ClientSockMsgCallBack m_MsgCB = NULL;
                    protected:
                        void __callDisconnectCallBack(){
                            if(this->m_DisconnCB != NULL){
                                this->m_DisconnCB(this);
                            }
                        }
                        void __callMsgCallBack(const std::vector<byte>& Data){
                            if(this->m_MsgCB != NULL){
                                this->m_MsgCB(Data,this);
                            }
                        }
                    public:
                        virtual void Disconnect() = 0;
                        virtual void SendMsg(const std::string& Message) = 0;
                        virtual void SendMsg(const std::vector<byte>& Data) = 0;
                        virtual bool isConnected() = 0;
                        void setOnDisconnectCallBack(ClientSockDisconnectCallBack newCallBack){
                            this->m_DisconnCB = newCallBack;
                        }
                        ClientSockDisconnectCallBack getOnDisconnectCallBack(){
                            return this->m_DisconnCB;
                        }
                        void setOnMessageCallBack(ClientSockMsgCallBack newCallBack){
                            this->m_MsgCB = newCallBack;
                        }
                        ClientSockMsgCallBack getOnMessageCallBack(){
                            return this->m_MsgCB;
                        }
                };

                class ClientSocket;
                typedef void(*ClientSocketConnectCallBack)(bool, ClientSocket*);
                class ClientSocket : public SingleConnectionSocket{
                    private:
                        ClientSocketConnectCallBack m_ConnectedCallBack = NULL;
                    protected:
                        void __callConnectedCallBack(bool Succeed){
                            if(this->m_ConnectedCallBack != NULL){
                                this->m_ConnectedCallBack(Succeed, this);
                            }
                        }
                    public:
                        virtual void Connect() = 0;
                        void setOnConnectedCallBack(ClientSocketConnectCallBack newCallBack){
                            this->m_ConnectedCallBack = newCallBack;
                        }
                        ClientSocketConnectCallBack getOnConnectedCallBack(){
                            return this->m_ConnectedCallBack;
                        }
                };

                class ServerSocket;
                typedef void(*ServerSockNewConnectionCallBack)(ServerSocket*, SingleConnectionSocket*);
                typedef void(*ServerSockStartListenCallBack)(bool, ServerSocket*);
                typedef void(*ServerSockStopListenCallBack)(ServerSocket*);
                class ServerSocket : public BaseSocket{
                    private:
                        ServerSockNewConnectionCallBack m_ConnectionCallBack = NULL;
                        ServerSockStartListenCallBack m_StartListenCallBack = NULL;
                        ServerSockStopListenCallBack m_StopListenCallBack = NULL;
                    protected:
                        void __callOnConnectionCallBack(SingleConnectionSocket* SingleConnectionPtr){
                            if(this->m_ConnectionCallBack != NULL){
                                this->m_ConnectionCallBack(this,SingleConnectionPtr);
                            }
                        }
                        void __callOnStartListenCallBack(bool Succeed){
                            if(this->m_StartListenCallBack != NULL){
                                this->m_StartListenCallBack(Succeed,this);
                            }
                        }
                        void __callOnStopListenCallBack(){
                            if(this->m_StopListenCallBack != NULL){
                                this->m_StopListenCallBack(this);
                            }
                        }
                    public:
                        void setOnConnectionCallBack(ServerSockNewConnectionCallBack newCallBack){
                            this->m_ConnectionCallBack = newCallBack;
                        }
                        ServerSockNewConnectionCallBack getOnConnectionCallBack(){
                            return this->m_ConnectionCallBack;
                        }
                        void setOnStartListenCallBack(ServerSockStartListenCallBack newCallBack){
                            this->m_StartListenCallBack = newCallBack;
                        }
                        ServerSockStartListenCallBack getOnStartListenCallBack(){
                            return this->m_StartListenCallBack;
                        }
                        void setOnStopListenCallBack(ServerSockStopListenCallBack newCallBack){
                            this->m_StopListenCallBack = newCallBack;
                        }
                        ServerSockStopListenCallBack getOnStopListenCallBack(){
                            return this->m_StopListenCallBack;
                        }

                };
            }
        }
    }
#endif