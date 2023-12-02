class Marker {
    private:
        bool is_food=false;
        float x, y, span=10;
    public:
        Marker() : x(0), y(0) {}
  Marker(float x_in, float y_in) : x(x_in), y(y_in) {}
  Marker(float x_in, float y_in, bool f_in) : x(x_in), y(y_in), is_food(f_in) {}

  float getX() { return x; }
  float getY() { return y; }
  float getSpan() { return span; }
  bool isFood() { return is_food; }
        void found_food() { is_food = true; }
        void decrement_span() { span--; }
        explicit operator bool() { return is_food; }
};
