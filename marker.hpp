

class Marker {
    private:
        bool is_food=false;
        float x, y, span=10;
    public:
        Marker() : x(0), y(0) {}
        Marker(float x_in, float y_in) : x(x_in), y(y_in) {}

        void found_food() { is_food = true; }
        void decrement_span() { span--; }
        explicit operator bool() { return is_food; }
};