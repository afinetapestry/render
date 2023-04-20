#pragma once

#include <chrono>
#include <iostream>
#include <tuple>

using day_t = std::chrono::duration<long long, std::ratio<3600 * 24>>;

template<typename> struct duration_traits {};
#define DURATION_TRAITS(Duration, Singular, Plural) \
	template<> struct duration_traits<Duration> { \
		constexpr static const char* singular = Singular; \
		constexpr static const char* plural = Plural; \
	}

DURATION_TRAITS(std::chrono::milliseconds, "millisecond", "milliseconds");
DURATION_TRAITS(std::chrono::seconds, "second", "seconds");
DURATION_TRAITS(std::chrono::minutes, "minute", "minutes");
DURATION_TRAITS(std::chrono::hours, "hour", "hours");
DURATION_TRAITS(day_t, "day", "days");

using divisions = std::tuple<std::chrono::milliseconds,
    std::chrono::seconds,
    std::chrono::minutes,
    std::chrono::hours,
    day_t>;

namespace detail {
    template<typename...> struct print_duration_impl_ {};

    template<typename Head, typename... Tail>
    struct print_duration_impl_<Head, Tail...> {
        template <typename Duration>
        static bool print(std::ostream & os, Duration & dur) {
            const auto started_printing = print_duration_impl_<Tail...>::print(os, dur);

            const auto n = std::chrono::duration_cast<Head>(dur);
            const auto count = n.count();

            if (count == 0) {
                return started_printing;
            }

            if (started_printing) {
                os << ' ';
            }

            using traits = duration_traits<Head>;
            os << count << ' ' << (count == 1 ? traits::singular : traits::plural);
            dur -= n;

            return true;
        }
    };

    template<> struct print_duration_impl_<> {
        template <typename Duration>
        static bool print(std::ostream & os, Duration & dur) {
            return false;
        }
    };

    template<typename...> struct print_duration {};

    template<typename... Args>
    struct print_duration<std::tuple<Args...>> {
        template<typename Duration>
        static void print(std::ostream & os, Duration dur) {
            print_duration_impl_<Args...>::print(os, dur);
        }
    };
}

template<typename Rep, typename Period>
std::ostream & operator<<(std::ostream & os, const std::chrono::duration<Rep, Period> & dur) {
    detail::print_duration<divisions>::print(os, dur);
    return os;
}
