#include "Review.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

unsigned Review::nextReviewId = 1;

Review::Review(unsigned userId, const std::string& fragranceName, const std::string& comment, double rating)
    : reviewId(nextReviewId++), userId(userId), fragranceName(fragranceName), comment(comment), rating(rating)
{
    if (rating < 0.0 || rating > 5.0)
        throw std::invalid_argument("Rating must be between 0 and 5.");
    if (comment.size() > 1000)
        throw std::invalid_argument("Comment must be at most 1000 characters.");
}

Review::Review(unsigned id, unsigned userId, const std::string& fragranceName, const std::string& comment, double rating)
    : reviewId(id), userId(userId), fragranceName(fragranceName), comment(comment), rating(rating)
{
    if (id >= nextReviewId) nextReviewId = id + 1;
}

unsigned Review::getId() const { return reviewId; }
unsigned Review::getUserId() const { return userId; }
double Review::getRating() const { return rating; }
const std::string& Review::getComment() const { return comment; }
const std::string& Review::getFragranceName() const { return fragranceName; }

void Review::show() const {
    std::cout << "  [Review #" << reviewId << "] "
        << "User #" << userId << " | Rating: " << rating << "/5\n"
        << "  Comment: " << comment << "\n";
}

std::string Review::serialize() const {
    std::ostringstream oss;
    oss << reviewId << "|" << userId << "|" << fragranceName
        << "|" << rating << "|" << comment;
    return oss.str();
}

Review Review::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    unsigned id, uid;
    std::string fname, comment;
    double rating;

    std::getline(iss, token, '|'); id = std::stoul(token);
    std::getline(iss, token, '|'); uid = std::stoul(token);
    std::getline(iss, fname, '|');
    std::getline(iss, token, '|'); rating = std::stod(token);
    std::getline(iss, comment);

    return Review(id, uid, fname, comment, rating);
}

void Review::setNextId(unsigned id) { if (id >= nextReviewId) nextReviewId = id + 1; }