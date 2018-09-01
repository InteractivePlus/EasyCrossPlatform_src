#ifndef __EASYCROSSPLATFORM_FILE_INFORMATIONFILTERHPP_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_INFORMATIONFILTERHPP_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    #include <EasyCrossPlatform/Templates/DataCarrier.hpp>
    #include <EasyCrossPlatform/Templates/SocketTemplate.hpp>

    namespace EasyCrossPlatform{
        namespace Templates{
            namespace InformationFilters{
                class OneWayInformationFilter;
                typedef void(*OneWayInformationFilterMsgCallBack)(std::vector<byte>&, OneWayInformationFilter*);
                class OneWayInformationFilter : public EasyCrossPlatform::Templates::VoidPtrDataCarrier{
                    private:
                        OneWayInformationFilterMsgCallBack m_InfoCallBack = NULL;
                    protected:
                        void __callMsgCallBack(std::vector<byte>& Data){
                            if(this->m_InfoCallBack != NULL){
                                this->m_InfoCallBack(Data,this);
                            }
                        }
                    public:
                        virtual void feedData(const std::string& Data) = 0;
                        virtual void feedData(const std::vector<byte>& Data) = 0;
                        virtual std::string getFilterTypeString() = 0;
                        void setMsgCallBack(OneWayInformationFilterMsgCallBack newCallBack){
                            this->m_InfoCallBack = newCallBack;
                        }
                        OneWayInformationFilterMsgCallBack getMsgCallBack(){
                            return this->m_InfoCallBack;
                        }
                };
                class TwoWayInformationFilter;
                enum InformationType{
                    Local,
                    Remote
                };
                typedef void(*TwoWayInformationFilterMsgCallBack)(std::vector<byte>&, InformationType, TwoWayInformationFilter*);
                class TwoWayInformationFilter : public EasyCrossPlatform::Templates::VoidPtrDataCarrier{
                    private:
                        TwoWayInformationFilterMsgCallBack m_LocalInfoCallBack = NULL;
                        TwoWayInformationFilterMsgCallBack m_RemoteInfoCallBack = NULL;
                    protected:
                        void __callLocalMsgCallBack(std::vector<byte>& Data){
                            if(this->m_LocalInfoCallBack != NULL){
                                this->m_LocalInfoCallBack(Data,InformationType::Local,this);
                            }
                        }
                        void __callRemoteMsgCallBack(std::vector<byte>& Data){
                            if(this->m_RemoteInfoCallBack != NULL){
                                this->m_RemoteInfoCallBack(Data,InformationType::Remote,this);
                            }
                        }
                    public:
                        virtual void feedLocalData(const std::string Data) = 0;
                        virtual void feedLocalData(const std::vector<byte>& Data) = 0;
                        virtual void feedRemoteData(const std::string&) = 0;
                        virtual void feedRemoteData(const std::vector<byte>& Data) = 0;
                        virtual std::string getFilterTypeString() = 0;
                        void setLocalMsgCallBack(TwoWayInformationFilterMsgCallBack newCallBack){
                            this->m_LocalInfoCallBack = newCallBack;
                        }
                        TwoWayInformationFilterMsgCallBack getMsgCallBack(){
                            return this->m_LocalInfoCallBack;
                        }
                        void setRemoteMsgCallBack(TwoWayInformationFilterMsgCallBack newCallBack){
                            this->m_RemoteInfoCallBack = newCallBack;
                        }
                        TwoWayInformationFilterMsgCallBack getMsgCallBack(){
                            return this->m_RemoteInfoCallBack;
                        }
                };
                
                class AutoFeedTwoWayInformationFilter : public EasyCrossPlatform::Templates::Sockets::SingleConnectionSocket{
                    private:
                        static void __SocketErrorCallBack(int ErrNo, const std::string& ErrDescription, EasyCrossPlatform::Templates::Sockets::BaseSocket* SocketPtr){
                            AutoFeedTwoWayInformationFilter* myAutoFeedFilter = (AutoFeedTwoWayInformationFilter*) SocketPtr->getCarriedData();
                            myAutoFeedFilter->__callErrorCallBack(ErrNo,ErrDescription);
                        }
                        static void __SocketMsgCallBack(const std::vector<byte>& Data, EasyCrossPlatform::Templates::Sockets::SingleConnectionSocket* SocketPtr){
                            AutoFeedTwoWayInformationFilter* myAutoFeedFilter = (AutoFeedTwoWayInformationFilter*) SocketPtr->getCarriedData();
                            if(myAutoFeedFilter->BindedFilter != NULL){
                                myAutoFeedFilter->BindedFilter->feedRemoteData(Data);
                            }else{
                                myAutoFeedFilter->__callMsgCallBack(Data);
                            }
                        }
                        static void __SocketDisconnectCallBack(EasyCrossPlatform::Templates::Sockets::SingleConnectionSocket* SocketPtr){
                            AutoFeedTwoWayInformationFilter* myAutoFeedFilter = (AutoFeedTwoWayInformationFilter*) SocketPtr->getCarriedData();
                            myAutoFeedFilter->__callDisconnectCallBack();
                        }
                        static void __FilterLocalMsgCallBack(std::vector<byte>& Data, InformationType DataType, TwoWayInformationFilter* FilterPtr){
                            AutoFeedTwoWayInformationFilter* myAutoFeedFilter = (AutoFeedTwoWayInformationFilter*) FilterPtr->getCarriedData();
                            if(myAutoFeedFilter->BindedSocket != NULL){
                                myAutoFeedFilter->BindedSocket->SendMsg(Data);
                            }
                        }
                        static void __FilterRemoteMsgCallBack(std::vector<byte>& Data, InformationType DataType, TwoWayInformationFilter* FilterPtr){
                            AutoFeedTwoWayInformationFilter* myAutoFeedFilter = (AutoFeedTwoWayInformationFilter*) FilterPtr->getCarriedData();
                            myAutoFeedFilter->__callMsgCallBack(Data);
                        }
                    protected:
                        EasyCrossPlatform::Templates::Sockets::SingleConnectionSocket* BindedSocket = NULL;
                        TwoWayInformationFilter* BindedFilter = NULL;
                        void __setBindSocketCallBack(){
                            if(this->BindedSocket != NULL){
                                this->BindedSocket->setCarriedData(this);
                                this->BindedSocket->setOnDisconnectCallBack(AutoFeedTwoWayInformationFilter::__SocketDisconnectCallBack);
                                this->BindedSocket->setOnErrorCallBack(AutoFeedTwoWayInformationFilter::__SocketErrorCallBack);
                                this->BindedSocket->setOnMessageCallBack(AutoFeedTwoWayInformationFilter::__SocketMsgCallBack);
                            }
                        }
                        void __setBindFilterCallBack(){
                            if(this->BindedFilter != NULL){
                                this->BindedFilter->setCarriedData(this);
                                this->BindedFilter->setLocalMsgCallBack(AutoFeedTwoWayInformationFilter::__FilterLocalMsgCallBack);
                                this->BindedFilter->setRemoteMsgCallBack(AutoFeedTwoWayInformationFilter::__FilterRemoteMsgCallBack);
                            }
                        }
                    public:
                        AutoFeedTwoWayInformationFilter(){
                            this->BindedFilter = NULL;
                            this->BindedSocket = NULL;
                        }
                        AutoFeedTwoWayInformationFilter(EasyCrossPlatform::Templates::Sockets::SingleConnectionSocket* BindingSocket, TwoWayInformationFilter* BindingFilter){
                            this->bindSocket(BindingSocket);
                            this->bindFilter(BindingFilter);
                        }
                        AutoFeedTwoWayInformationFilter(AutoFeedTwoWayInformationFilter&& OldAutoFeedFilter){
                            this->bindSocket(OldAutoFeedFilter.BindedSocket);
                            this->bindFilter(OldAutoFeedFilter.BindedFilter);
                            OldAutoFeedFilter.BindedSocket = NULL;
                            OldAutoFeedFilter.BindedFilter = NULL;
                        }
                        std::string getSocketTypeString(){
                            std::string SocketTypeString = std::string();
                            if(this->BindedFilter != NULL){
                                SocketTypeString += this->BindedFilter->getFilterTypeString();
                            }
                            if(this->BindedFilter != NULL && this->BindedSocket != NULL){
                                SocketTypeString += "::";
                            }
                            if(this->BindedSocket != NULL){
                                SocketTypeString += this->BindedSocket->getSocketTypeString();
                            }
                            return SocketTypeString;
                        }
                        void Disconnect(){
                            if(this->BindedSocket != NULL){
                                this->BindedSocket->Disconnect();
                            }
                        }
                        void SendMsg(std::string Message){
                            if(this->BindedFilter != NULL){
                                this->BindedFilter->feedLocalData(Message);
                            }else if(this->BindedSocket != NULL){
                                this->BindedSocket->SendMsg(Message);
                            }
                        }
                        void SendMsg(std::vector<byte> Data){
                            if(this->BindedFilter != NULL){
                                this->BindedFilter->feedLocalData(Data);
                            }else if(this->BindedSocket != NULL){
                                this->BindedSocket->SendMsg(Data);
                            }
                        }
                        bool isConnected(){
                            if(this->BindedSocket != NULL){
                                return this->BindedSocket->isConnected();
                            }else{
                                return false;
                            }
                        }
                        void bindSocket(EasyCrossPlatform::Templates::Sockets::SingleConnectionSocket* BindingSocket){
                            this->BindedSocket = BindingSocket;
                            this->__setBindSocketCallBack();
                        }
                        EasyCrossPlatform::Templates::Sockets::SingleConnectionSocket* getBindedSocket(){
                            return this->BindedSocket;
                        }
                        void bindFilter(TwoWayInformationFilter* BindingFilter){
                            this->BindedFilter = BindingFilter;
                            this->__setBindFilterCallBack();
                        }
                        TwoWayInformationFilter* getBindedFilter(){
                            return this->BindedFilter;
                        }
                };
            }
        }
    }
#endif