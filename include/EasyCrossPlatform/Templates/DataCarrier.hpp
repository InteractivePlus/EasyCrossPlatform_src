#ifndef __EASYCROSSPLATFORM_FILE_DATACARRIERHPP_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_DATACARRIERHPP_INCLUDED__
    namespace EasyCrossPlatform{
        namespace Templates{
            template <typename T>
            class DataCarrier{
                private:
                    T CustomData;
                protected:
                    
                public:
                    T getCarriedData(){
                        return CustomData;
                    }
                    void setCarriedData(T Value){
                        this.CustomData = Value;
                    }
            };
            typedef DataCarrier<void*> VoidPtrDataCarrier;
        }
    }
#endif