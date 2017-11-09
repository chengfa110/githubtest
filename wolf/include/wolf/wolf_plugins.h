#ifndef WOLF_PLUGINS_H_
#define WOLF_PLUGINS_H_

    namespace wolf_plugins
    {
      class DistanceWolf : public wolf_base::RegularWolf
      {
        public:
          DistanceWolf(){}
          virtual void start(int num) 
          {
            
          }
          
        protected:
      };
      
      class DecideWolf : public wolf_base::RegularWolf
      {
        public:
        virtual void start(int num) = 0;
        virtual ~DecideWolf(){}

        protected:
          DecideWolf(){}
      };
      
      class RunWolf : public wolf_base::RegularWolf
      {
        public:
        virtual void start(int num) = 0;
        virtual ~RunWolf(){}

        protected:
          RunWolf(){}
      };
    };
    #endif
