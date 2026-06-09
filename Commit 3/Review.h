#pragma once
#include <string>

class Review {
    unsigned reviewId;
    static unsigned nextReviewId;

    unsigned userId;
    std::string fragranceName;
    std::string comment;
    double rating;

public:
    Review(unsigned userId, const std::string& fragranceName, const std::string& comment, double rating);

    Review(unsigned id, unsigned userId, const std::string& fragranceName, const std::string& comment, double rating);

    unsigned getId() const;
    unsigned getUserId() const;
    double getRating() const;
    const std::string& getComment() const;
    const std::string& getFragranceName() const;

    void show() const;

    std::string serialize() const;
    static Review deserialize(const std::string& line);

    static void setNextId(unsigned id);
};
