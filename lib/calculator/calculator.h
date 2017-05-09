class Calculator {
  private:
    // Temperatures
    float t1, t2, t3, tDelta;

    // Relay states
    bool m1, m2, m3, m4, q1;

    // Button states
    bool s1;

  public:
    Calculator() {
      t1 = 0.0;
      t2 = 0.0;
      t3 = 0.0;
      tDelta = 0.0;
      m1 = false;
      m2 = false;
      m3 = false;
      m4 = false;
      q1 = false;
      s1 = false;
    }

    void setT1(float value);
    void setT2(float value);
    void setT3(float value);
    void setM1(float value);
    void setM2(float value);
    void setM3(float value);
    void setM4(float value);
    void setQ1(float value);
    void setS1(float value);

    float getT1();
    float getT2();
    float getT3();
    float getTDelta();
    bool getM1();
    bool getM2();
    bool getM3();
    bool getM4();
    bool getQ1();
    bool getS1();

    void calculate();
};
