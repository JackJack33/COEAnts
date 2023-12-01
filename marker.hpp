

class Marker {
    private:
        bool food=false;
        float x, y, span=10;
    public:
        Marker() : x(0), y(0) {}
        Marker(float x_in, float y_in, bool from_food) : x(x_in), y(y_in), food(from_food) {}

        void found_food() { food = true; }
        bool is_food() { return food; }
        int get_span() { return span; }
        void decrement_span() { span--; }
        bool in_fov(function<bool(float, float)> const &fov_func) { return fov_func(x, y); }
        float distance(float x_in, float y_in) { return std::sqrt((x - x_in) * (x - x_in) + (y - y_in) * (y - y_in)); }
};