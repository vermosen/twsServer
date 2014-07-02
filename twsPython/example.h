class example {

	public: 

		example() : a_(0.0), b_(0.0) {};

		double a_;
		double b_;
		
		// accessors
		double a() const { return a_; };
		double b() const { return b_; };

		void a(const double & val) { a_ = val; };
		void b(const double & val) { b_ = val; };
};