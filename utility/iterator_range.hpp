template <class Iterator>
class IteratorRange {
    Iterator begin_, end_;

public:
    IteratorRange(Iterator begin, Iterator end) : begin_{std::move(begin)}, end_{std::move(end)} {
        // no-op
    }

    template <class Container>
    explicit IteratorRange(const Container &container)
    : begin_{std::begin(container)}, end_(std::end(container)) {
        // no-op
    }

    auto begin() const {
        return begin_;
    }

    auto end() const {
        return end_;
    }
};
