class Box
{
   public:
             virtual double getVolume() = 0;
		double hshd()
		{
			length=0;
		}
   private:
                    double length;      // 长度
                    double breadth;     // 宽度                           
		    double height;      // 高度
};
