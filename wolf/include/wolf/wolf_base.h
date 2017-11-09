#ifndef WOLF_BASE_H_
#define WOLF_BASE_H_

namespace wolf_base
{
  class RegularWolf
  {
    public:
     virtual void start(int num) = 0;
     virtual ~RegularWolf(){}

    protected:
     RegularWolf(){}
  };

};

#endif
