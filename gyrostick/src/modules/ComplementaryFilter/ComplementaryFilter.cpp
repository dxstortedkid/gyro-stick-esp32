class ComplementaryFilter {
    private:
        float alpha; // Коэффициент фильтра (например, 0.98)
        float dt; // Шаг по времени в секундах
        float prev_measurements; // Сохраненный прошлый угол \theta[n-1]
    public:
    // Конструктор
    ComplementaryFilter(float init_alpha, float init_dt, float init_prev_measurements) {
        alpha = init_alpha;
        dt = init_dt;
        prev_measurements = init_prev_measurements;
    }

    // Основной расчёт(метод обновления)
    float update(float gyro_rate, float acc_angle){
        float tetha = alpha *(prev_measurements + (gyro_rate * dt)) + ((1 - alpha) * acc_angle);
        return prev_measurements = tetha;
    }

    // Сброс памяти фильтра
    void reset() {
        prev_measurements = 0.0f;
    }

    // Выставка параметров
    void setFilterParam(float new_alpha, float new_dt){
        alpha = new_alpha;
        dt = new_dt;
    }

};