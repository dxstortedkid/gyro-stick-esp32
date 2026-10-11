class HighPassFilter{
    private:
            float alpha;
            float prev_x;
            float prev_y;

    public:
        HighPassFilter(float initial_aplha){
            alpha = initial_aplha;
            prev_x = 0.0f;
            prev_y = 0.0f;
        }
        
        void setAlpha(float new_aplha){
            alpha = new_aplha;
        }

        float update(float current_x){
            float current_y = alpha * (prev_y + current_x - prev_x);
            prev_x = current_x;
            prev_y = current_y;
            return current_y;
        }

        void reset(){
            prev_x = 0.0f;
            prev_y = 0.0f;
        }

};