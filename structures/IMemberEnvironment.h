//
// Created by slyfox on 16.05.20.
//

#pragma once

#include <random>

class IMemberEnvironment {
public:
    bool roll_to_die()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::bernoulli_distribution dst(m_die_probability);
        return dst(gen);
    }

    bool roll_to_recover()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::bernoulli_distribution dst(m_recover_probability);
        return dst(gen);
    }

protected:
    double m_die_probability = 0.001;
    double m_recover_probability = 0.001;
};
