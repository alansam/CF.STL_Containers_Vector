//
//  vectors.cpp
//  CF.STL_Containers_Vector
//
//  Created by Alan Sampson on 3/21/21.
//
//  MARK: - Reference.
//  @see: https://en.cppreference.com/w/cpp/container
//  @see: https://en.cppreference.com/w/cpp/container/vector
//  @see: https://en.cppreference.com/w/cpp/container/vector_bool
//

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <span>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <compare>
#include <cstddef>

using namespace std::literals::string_literals;

//  MARK: - Definitions

//  MARK: - Local Constants.
namespace konst {

auto delimiter(char const dc = '-', size_t sl = 80) -> std::string const {
  auto const dlm = std::string(sl, dc);
  return dlm;
}

static
auto const dlm = delimiter();

static
auto const dot = delimiter('.');

} /* namespace konst */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace valc
namespace valc {

template <class T>
struct Mallocator {
  typedef T value_type;

  Mallocator () = default;
  template <class U>
  constexpr Mallocator(const Mallocator <U> &) noexcept {
    std::cout << "In: "s << __func__ << std::endl;
  }

  [[nodiscard]]
  T * allocate(std::size_t n_) {
    std::cout << "In: "s << __func__
              << ", request size: "s << n_
              << ", request typeid: "s << typeid(T).name()
              << ", type size: "s << sizeof(T)
              << ", bytes: " << n_ * sizeof(T)
              << std::endl;
    if (n_ > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
      throw std::bad_alloc();
    }

    if (auto pm = static_cast<T *>(std::malloc(n_ * sizeof(T)))) {
      report(pm, n_);
      return pm;
    }

    throw std::bad_alloc();
  }

  void deallocate(T * pm, std::size_t n_) noexcept {
    std::cout << "In: "s << __func__ << std::endl;
    report(pm, n_, 0);
    std::free(pm);
  }

private:
  void report(T * pm, std::size_t n_, bool alloc = true) const {
    std::cout << "In: "s << __func__ << std::endl;
    std::cout << (alloc ? "Alloc: "s : "Dealloc: "s) << sizeof(T) * n_
              << " bytes at "s << std::hex << std::showbase
              << reinterpret_cast<void*>(pm) << std::dec << std::noshowbase
              << '\n';
  }
};

template <class T, class U>
bool operator==(const Mallocator <T> & lhs, const Mallocator <U> & rhs) {
  std::cout << "In: "s << __func__ << ' '
            << typeid(T).name() << " == "s << typeid(U).name() << std::endl;
  return typeid(T) == typeid(U) ? true : false;
}

template <class T, class U>
bool operator!=(const Mallocator <T> & lhs, const Mallocator <U> & rhs) {
  std::cout << "In: "s << __func__ << ' '
            << typeid(T).name() << " != "s << typeid(U).name() << std::endl;
  return typeid(T) != typeid(U) ? true : false;
}

} /* namespace valc */

#if (__cplusplus > 201707L)
#endif  /* (__cplusplus > 201707L) */

//  MARK: - Function Prototype.
int C_vector(int argc, const char * argv[]);
int C_vector_bool(int argc, const char * argv[]);

//  MARK: - Implementation.
/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "CF.STL_Containers_Vector\n";
  std::cout << "C++ Version: "s << __cplusplus << std::endl;

  std::cout << '\n' << konst::dlm << std::endl;
  C_vector(argc, argv);

  std::cout << '\n' << konst::dlm << std::endl;
  C_vector_bool(argc, argv);

  return 0;
}

//  MARK: - C_vector
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  MARK: namespace vec
namespace vec {
template<typename T>
std::ostream & operator<<(std::ostream & strm, const std::vector<T> & vec)
{
  strm.put('[');
  char comma[3] = {'\0', ' ', '\0'};
  for (auto const & el : vec) {
    strm << comma << el;
    comma[0] = ',';
  }

  return strm << ']';
}

} /* namespace vec */

namespace vecpop {

template<typename T>
void print(T const & xs) {
  std::cout << "[ "s;
  for (auto const & x_ : xs) {
    std::cout << x_ << ' ';
  }
  std::cout << "]\n"s;
}

} /* namespace vecpop */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace vecrsv
namespace vecrsv {

// minimal C++11 allocator with debug output
template <class Tp>
struct NAlloc {
  typedef Tp value_type;

  NAlloc() = default;

  template <class T> NAlloc(const NAlloc<T> &) {
    std::cout << "In: "s << __func__ << std::endl;
  }

  Tp * allocate(std::size_t nv) {
    std::cout << "In: "s << __func__
              << ", request size: "s << nv
              << ", request typeid: "s << typeid(Tp).name()
              << ", type size: "s << sizeof(Tp)
              << ", bytes: " << nv * sizeof(Tp)
              << std::endl;
    nv *= sizeof(Tp);
    Tp * p_typ = static_cast<Tp *>(::operator new(nv));
    std::cout << "allocating "s << nv
              << " bytes at address "s << p_typ
              << std::endl;
    return p_typ;
  }

  void deallocate(Tp * p_typ, std::size_t nv) {
    std::cout << "In: "s << __func__ << std::endl;
    std::cout << "deallocating "s << nv * sizeof * p_typ
              << " bytes from address "s << p_typ
              << std::endl;
    ::operator delete(p_typ);
  }
};

template <class T, class U>
bool operator==(const NAlloc<T> &, const NAlloc<U> &) {
  std::cout << "In: "s << __func__ << std::endl;
  return true;
}

template <class T, class U>
bool operator!=(const NAlloc<T> &, const NAlloc<U> &) {
  std::cout << "In: "s << __func__ << std::endl;
  return false;
}

} /* namespace vecrsv */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace vecswp
namespace vecswp {

template<class Os, class Co> Os & operator<<(Os & os, const Co& co) {
  os << "{"s;
  for (auto const & i : co) {
    os << ' ' << i;
  }
  return os << " } "s;
}

} /* namespace vecswp */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: C_vector()
 */
int C_vector(int argc, const char * argv[]) {
  std::cout << "In "s << __func__ << std::endl;

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector"s << '\n';
  {
    // Create a vector containing integers
    std::vector<int> vnr = { 7, 5, 16, 8, };

    // Add two more integers to vector
    vnr.push_back(25);
    vnr.push_back(13);

    // Print out the vector
    std::cout << "v = { "s;
    for (int nr : vnr) {
      std::cout << nr << ", "s;
    }
    std::cout << "}; \n"s;
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Constructors
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - constructor"s << '\n';
  {
    using namespace vec;

    // c++11 initializer list syntax:
    std::vector<std::string> words1 { "the"s, "frogurt"s, "is"s, "also"s, "cursed"s, };
    std::cout << "words1: "s << words1 << '\n';

    // words2 == words1
    std::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: "s << words2 << '\n';

    // words3 == words1
    std::vector<std::string> words3(words1);
    std::cout << "words3: "s << words3 << '\n';

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    std::vector<std::string> words4(5, "Mo");
    std::cout << "words4: "s << words4 << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - constructor, custom allocator"s << '\n';
  {
    auto cc(0ul);
    auto constexpr cc_max(10ul);
    auto sp(4);
    auto prtnumeric = [&cc, &sp](auto el) {
      std::cout << std::setw(sp) << el
      << (++cc % cc_max == 0 ? "\n"s : ""s);
    };

    std::vector<int, valc::Mallocator<int>> vnr(8);
    std::cout << "initial size: "s << std::setw(4) << vnr.size()
              << ", capacity: "s << std::setw(6) << vnr.capacity()
              << '\n';

    if (!vnr.empty()) {
      std::cout << vnr.data() << '\n';
      cc = 0ul;
      sp = 4ul;
      std::for_each(vnr.cbegin(), vnr.cend(), prtnumeric);
      std::cout << '\n';
    }
    std::cout << "add a value.\n"s;
    vnr.push_back(42);

    std::cout << "add a value.\n"s;
    for (auto nr : { -42, 21, 77, -0, -1, 0, 666, 33, -99, 3, }) {
      std::cout << "adding "s << std::setw(4) << nr << '\n';
      vnr.push_back(nr);
    }

    std::cout << vnr.data() << '\n';
    std::cout << "new size: "s << std::setw(4) << vnr.size()
              << ", capacity: "s << std::setw(6) << vnr.capacity()
              << '\n';
    cc = 0ul;
    sp = 4ul;
    std::for_each(vnr.cbegin(), vnr.cend(), prtnumeric);
    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Container functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - operator="s << '\n';
  {
    auto display_sizes = [](std::string comment,
                            std::vector<int> const & nums1,
                            std::vector<int> const & nums2,
                            std::vector<int> const & nums3) {
      std::cout << comment
                << " nums1: "s << nums1.size() << ','
                << " nums2: "s << nums2.size() << ','
                << " nums3: "s << nums3.size() << '\n';
    };

    auto display = [](std::string comment, std::vector<int> const & vec) {
      std::cout << comment << "{ "s;
      for (int el : vec) {
        std::cout << el << ' ';
      }
      std::cout << "}\n"s;
    };

    std::vector<int> nums1 { 3, 1, 4, 6, 5, 9, };
    std::vector<int> nums2;
    std::vector<int> nums3;

    display_sizes("Initially:\n"s, nums1, nums2, nums3);
//    display_sizes(co, nums1, nums2, nums3);

    // copy assignment copies data from nums1 to nums2
    nums2 = nums1;

    display_sizes("After assignment:\n"s, nums1, nums2, nums3);

    // move assignment moves data from nums1 to nums3,
    // modifying both nums1 and nums3
    nums3 = std::move(nums1);

    display_sizes("After move assignment:\n"s, nums1, nums2, nums3);

    display("Now nums3 = "s, nums3);

    // copy assignment of an initializer_list copies data to nums3
    nums3 = { 1, 2, 3, };

    display("After assignment of initializer_list \n nums3 = "s, nums3);
    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - assign"s << '\n';
  {
    std::vector<char> characters;

    auto print_vector = [&](){
      for (char ch : characters) {
        std::cout << ch << ' ';
      }
      std::cout << '\n';
    };

    characters.assign(5, 'a');
    print_vector();

    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_vector();

    characters.assign({ 'C', '+', '+', '1', '1', });
    print_vector();
    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - get_allocator"s << '\n';
  {
    std::vector<long> vb1(8);
    std::vector<long, valc::Mallocator<long>> vb2(8);
    std::vector<long, valc::Mallocator<long>> vb3(8);
    std::vector<uint8_t, valc::Mallocator<uint8_t>> vb4(8);

    [[maybe_unused]]
    auto vb1_al = vb1.get_allocator();
    auto vb2_al = vb2.get_allocator();
    auto vb3_al = vb3.get_allocator();
    auto vb4_al = vb4.get_allocator();

    std::cout << std::boolalpha;
    auto vb2_al_eq_vb2_al  = vb2_al == vb2_al;
    auto vb2_al_neq_vb2_al = vb2_al != vb2_al;
    std::cout << "vb2_al == vb2_al: "s << vb2_al_eq_vb2_al  << '\n';
    std::cout << "vb2_al != vb2_al: "s << vb2_al_neq_vb2_al << '\n';

    auto vb2_al_eq_vb3_al  = vb2_al == vb3_al;
    auto vb2_al_neq_vb3_al = vb2_al != vb3_al;
    std::cout << "vb2_al == vb3_al: "s << vb2_al_eq_vb3_al  << '\n';
    std::cout << "vb2_al != vb3_al: "s << vb2_al_neq_vb3_al << '\n';

    auto vb2_al_eq_vb4_al  = vb2_al == vb4_al;
    auto vb2_al_neq_vb4_al = vb2_al != vb4_al;
    std::cout << "vb2_al == vb4_al: "s << vb2_al_eq_vb4_al  << '\n';
    std::cout << "vb2_al != vb4_al: "s << vb2_al_neq_vb4_al << '\n';
    std::cout << std::noboolalpha;
    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Element access
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - at"s << '\n';
  {
    std::vector<int> data = { 1, 2, 4, 5, 5, 6, };

    // Set element 1
    data.at(1) = 88;

    // Read element 2
    std::cout << "Element at index 2 has value "s << data.at(2) << '\n';

    std::cout << "data size = "s << data.size() << '\n';

    try {
      // Set element 6
      data.at(6) = 666;
    }
    catch (std::out_of_range const & exc) {
      std::cout << exc.what() << '\n';
    }

    // Print final values
    std::cout << "data:"s;
    std::for_each(data.cbegin(), data.cend(), [](auto elem) {
      std::cout << ' ' << elem;
    });
    std::cout << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - operator[]"s << '\n';
  {
    std::vector<int> numbers { 2, 4, 6, 8, };

    std::cout << "Second element: "s << numbers[1] << '\n';

    numbers[0] = 5;

    std::cout << "All numbers:"s;
    std::for_each(numbers.cbegin(), numbers.cend(), [](auto elem) {
      std::cout << ' ' << elem;
    });
    std::cout << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - front, back"s << '\n';
  {
    {
      std::vector<char> letters { 'o', 'm', 'g', 'w', 't', 'f', };

      if (!letters.empty()) {
        std::cout << "The first character is '"s << letters.front() << "'.\n"s;
      }
    }
    std::cout << '\n';

    {
      std::vector<char> letters { 'a', 'b', 'c', 'd', 'e', 'f', };

      if (!letters.empty()) {
        std::cout << "The last character is '"s << letters.back() << "'.\n"s;
      }
    }

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - data"s << '\n';
  {
    auto pointer_func = [](const int* p, std::size_t size) {
      std::cout << "data = "s;
      for (std::size_t i_ = 0; i_ < size; ++i_) {
        std::cout << p[i_] << ' ';
      }
      std::cout << '\n';
    };
     
    auto span_func = [](std::span<const int> data) {  // since C++20
      std::cout << "data = "s;
      for (const int e_ : data) {
        std::cout << e_ << ' ';
      }
      std::cout << '\n';
    };

    std::vector<int> container { 1, 2, 3, 4, };

    // Prefer container.data() over &container[0]
    pointer_func(container.data(), container.size());

    // std::span (C++20) is a safer alternative to separated pointer/size.
    span_func({container.data(), container.size()});

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Iterators
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - begin, end, etc."s << '\n';
  {
    std::cout << "std::vector - begin, end"s << '\n';
    {
      std::vector<int> nums { 1, 2, 4, 8, 16, };
      std::vector<std::string> fruits { "orange"s, "apple"s, "raspberry"s, };
      std::vector<char> empty;

      // Print vector.
      std::for_each(nums.begin(), nums.end(), [](const int n_) {
        std::cout << n_ << ' ';
      });
      std::cout << '\n';

      // Sums all integers in the vector nums (if any), printing only the result.
      std::cout << "Sum of nums: "s
                << std::accumulate(nums.begin(), nums.end(), 0) << '\n';

      // Prints the first fruit in the vector fruits, checking if there is any.
      if (!fruits.empty()) {
        std::cout << "First fruit: "s << *fruits.begin() << '\n';
      }

      if (empty.begin() == empty.end()) {
        std::cout << "vector 'empty' is indeed empty.\n"s;
      }
    }
    std::cout << '\n';

    std::cout << "std::vector - rbegin, rend"s << '\n';
    {
      std::vector<int> nums { 1, 2, 4, 8, 16, };
      std::vector<std::string> fruits { "orange"s, "apple"s, "raspberry"s, };
      std::vector<char> empty;

      // Print vector.
      std::for_each(nums.rbegin(), nums.rend(), [](const int n_) {
        std::cout << n_ << ' ';
      });
      std::cout << '\n';

      // Sums all integers in the vector nums (if any), printing only the result.
      std::cout << "Sum of nums: "s
                << std::accumulate(nums.rbegin(), nums.rend(), 0) << '\n';

      // Prints the first fruit in the vector fruits, checking if there is any.
      if (!fruits.empty()) {
        std::cout << "First fruit: "s << *fruits.rbegin() << '\n';
      }

      if (empty.rbegin() == empty.rend()) {
        std::cout << "vector 'empty' is indeed empty.\n"s;
      }
    }
    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Capaciity
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - empty"s << '\n';
  {
    std::cout << std::boolalpha;
    std::vector<int> numbers;
    std::cout << "Initially, numbers.empty(): "s
              << numbers.empty() << '\n';

    numbers.push_back(42);
    std::cout << "After adding elements, numbers.empty(): "s
              << numbers.empty() << '\n';
    std::cout << std::noboolalpha;

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - size"s << '\n';
  {
    std::vector<int> nums { 1, 3, 5, 7, };

    std::cout << "nums contains "s << nums.size() << " elements.\n"s;

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - max_size"s << '\n';
  {
    std::vector<char> svec;
    std::cout << "Maximum size of a 'vector' is "s << svec.max_size() << "\n"s;

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - reserve"s << '\n';
  {
    int sz = 100;
    std::cout << "using reserve: \n"s;
    {
      std::vector<int, vecrsv::NAlloc<int>> v1;
      v1.reserve(sz);
      for (int n_ = 0; n_ < sz; ++n_) {
        v1.push_back(n_);
      }
    }
    std::cout << "not using reserve: \n"s;
    {
      std::vector<int, vecrsv::NAlloc<int>> v1;
        for (int n_ = 0; n_ < sz; ++n_) {
          v1.push_back(n_);
        }
    }

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - capacity"s << '\n';
  {
    int sz = 200;
    std::vector<int> v1;

    auto cap = v1.capacity();
    std::cout << "initial capacity=" << cap << '\n';

    for (int n_ = 0; n_ < sz; ++n_) {
      v1.push_back(n_);
      if (cap != v1.capacity()) {
        cap = v1.capacity();
        std::cout << "new capacity="s << cap << '\n';
      }
    }

    std::cout << "final size="s << v1.size() << '\n';
    std::cout << "final capacity="s << v1.capacity() << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - shrink_to_fit"s << '\n';
  {
    std::vector<int> vec;
    std::cout << "Default-constructed capacity is "s << vec.capacity() << '\n';
    vec.resize(100);
    std::cout << "Capacity of a 100-element vector is "s << vec.capacity() << '\n';
    vec.resize(50);
    std::cout << "Capacity after resize(50) is " << vec.capacity() << '\n';
    vec.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is "s << vec.capacity() << '\n';
    vec.clear();
    std::cout << "Capacity after clear() is "s << vec.capacity() << '\n';
    vec.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is "s << vec.capacity() << '\n';
    for (int i_ = 1000; i_ < 1300; ++i_) {
      vec.push_back(i_);
    }
    std::cout << "Capacity after adding 300 elements is "s << vec.capacity() << '\n';
    vec.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is "s << vec.capacity() << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Modifiers
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - clear"s << '\n';
  {
    std::vector<int> container { 1, 2, 3, };
     
    auto print = [](const int& n) { std::cout << " " << n; };

    std::cout << "Before clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size()
              << ", Capacity=" << container.capacity() << '\n';

    std::cout << "Clear\n";
    container.clear();

    std::cout << "After clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size()
              << ", Capacity=" << container.capacity() << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - insert"s << '\n';
  {
    auto print_vec = [](const std::vector<int>& vec) {
        for (auto x_ : vec) {
             std::cout << ' ' << x_ ;
        }
        std::cout << '\n';
    };

    std::vector<int> vec(3, 100);
    print_vec(vec);

    auto it = vec.begin();
    it = vec.insert(it, 200);
    print_vec(vec);

    vec.insert(it, 2, 300);
    print_vec(vec);

    // "it" no longer valid, get a new one:
    it = vec.begin();

    std::vector<int> vec2(2, 400);
    vec.insert(it + 2, vec2.begin(), vec2.end());
    print_vec(vec);

    int arr[] = { 501, 502, 503, };
    vec.insert(vec.begin(), arr, arr + 3);
    print_vec(vec);

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - emplace"s << '\n';
  {
    struct Aempl {
      std::string str;

      Aempl(std::string str) : str(std::move(str))  {
        std::cout << " constructed\n"s;
      }

      Aempl(const Aempl & other) : str(other.str) {
        std::cout << " copy constructed\n"s;
      }

      Aempl(Aempl && other) : str(std::move(other.str)) {
        std::cout << " move constructed\n"s;
      }

      Aempl & operator=(const Aempl & other) {
        str = other.str;
        std::cout << " copy assigned\n"s;
        return *this;
      }

      Aempl & operator=(Aempl && other) {
        str = std::move(other.str);
        std::cout << " move assigned\n"s;
        return *this;
      }
    };

    std::vector<Aempl> container;
    // reserve enough place so vector does not have to resize
    container.reserve(10);
    std::cout << "construct 2 times Aempl:\n"s;
    Aempl two { "two"s };
    Aempl three { "three"s };

    std::cout << "emplace:\n"s;
    container.emplace(container.end(), "one"s);

    std::cout << "emplace with Aempl &:\n"s;
    container.emplace(container.end(), two);

    std::cout << "emplace with Aempl &&:\n"s;
    container.emplace(container.end(), std::move(three));

    std::cout << "content:\n"s;
    for (const auto & obj : container)
      std::cout << ' ' << obj.str;
    std::cout << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - emplace"s << '\n';
  {
    auto print_container = [](std::vector<int> const & ctr) {
      for (auto & nr : ctr) {
        std::cout << nr << ' ';
      }
      std::cout << '\n';
    };

    std::vector<int> container { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    print_container(container);

    container.erase(container.begin());
    print_container(container);

    container.erase(container.begin() + 2, container.begin() + 5);
    print_container(container);

    // Erase all even numbers (C++11 and later)
    for (auto it = container.begin(); it != container.end(); ) {
      if (*it % 2 == 0) {
        it = container.erase(it);
      }
      else {
        ++it;
      }
    }
    print_container(container);

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - push_back"s << '\n';
  {
    std::vector<std::string> letters;

    letters.push_back("abc"s);
    std::string str = "def"s;
    letters.push_back(std::move(str));

    std::cout << "vector holds: "s;
    for (auto && im : letters) {
      std::cout << std::quoted(im) << ' ';
    }
    std::cout << "\nMoved-from string holds "s << std::quoted(str) << '\n';
    std::cout << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - emplace_back"s << '\n';
  {
    struct President {
      std::string name;
      std::string country;
      int year;

      President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)),
          country(std::move(p_country)),
          year(p_year) {
        std::cout << "I am being constructed.\n"s;
      }

      President(President && other)
        : name(std::move(other.name)),
          country(std::move(other.country)),
          year(other.year) {
        std::cout << "I am being moved.\n"s;
      }

      President & operator=(const President & other) = default;
    };

    std::vector<President> elections;
    std::cout << "emplace_back:\n"s;
    auto& ref = elections.emplace_back("Nelson Mandela"s, "South Africa"s, 1994);
    assert(ref.year != 1984 && "uses a reference to the created object (C++17)");

    std::vector<President> reElections;
    std::cout << "\npush_back:\n"s;
    reElections.push_back(President("Franklin Delano Roosevelt"s, "the USA"s, 1936));

    std::cout << "\nContents:\n"s;
    for (President const& president: elections) {
      std::cout << president.name << " was elected president of "s
                << president.country << " in "s << president.year << ".\n"s;
    }

    for (President const& president: reElections) {
      std::cout << president.name << " was re-elected president of "s
                << president.country << " in "s << president.year << ".\n"s;
    }

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - pop_back"s << '\n';
  {
    std::vector<int> numbers;

    vecpop::print(numbers);

    numbers.push_back(5);
    numbers.push_back(3);
    numbers.push_back(4);

    vecpop::print(numbers);

    numbers.pop_back();

    vecpop::print(numbers);

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - resize"s << '\n';
  {
    auto print_element = [](auto & el) {
      std::cout << el << ' ';
    };
    auto print = [&](auto & container) {
      std::for_each(container.cbegin(), container.cend(), print_element);
    };

    std::vector<int> container = { 1, 2, 3, };
    std::cout << "The vector holds: "s;
    print(container);
    std::cout << '\n';

    container.resize(5);
    std::cout << "After resize up to 5: "s;
    print(container);
    std::cout << '\n';

    container.resize(2);
    std::cout << "After resize down to 2: "s;
    print(container);
    std::cout << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - swap"s << '\n';
  {
    using namespace vecswp;

    std::vector<int> a1{ 1, 2, 3, }, a2{ 4, 5, };

    auto it1 = std::next(a1.begin());
    auto it2 = std::next(a2.begin());

    int& ref1 = a1.front();
    int& ref2 = a2.front();

    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
    a1.swap(a2);
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';

    // Note that after swap the iterators and references stay associated with their
    // original elements, e.g. it1 that pointed to an element in 'a1' with value 2
    // still points to the same element, though this element was moved into 'a2'.

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Non-member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - std::operator== etc."s << '\n';
  {
    std::vector<int> alice{1, 2, 3};
    std::vector<int> bob{7, 8, 9, 10};
    std::vector<int> eve{1, 2, 3};

    std::cout << std::boolalpha;

    // Compare non equal containers
    std::cout << "alice == bob returns "s << (alice == bob) << '\n';
    std::cout << "alice != bob returns "s << (alice != bob) << '\n';
    std::cout << "alice <  bob returns "s << (alice < bob) << '\n';
    std::cout << "alice <= bob returns "s << (alice <= bob) << '\n';
    std::cout << "alice >  bob returns "s << (alice > bob) << '\n';
    std::cout << "alice >= bob returns "s << (alice >= bob) << '\n';

    std::cout << '\n';

    // Compare equal containers
    std::cout << "alice == eve returns "s << (alice == eve) << '\n';
    std::cout << "alice != eve returns "s << (alice != eve) << '\n';
    std::cout << "alice <  eve returns "s << (alice < eve) << '\n';
    std::cout << "alice <= eve returns "s << (alice <= eve) << '\n';
    std::cout << "alice >  eve returns "s << (alice > eve) << '\n';
    std::cout << "alice >= eve returns "s << (alice >= eve) << '\n';

#if (__cplusplus > 201707L)
    //`TODO:  add spaceship operator tests
    auto eq = std::is_eq(alice <=> eve);
    auto ne = std::is_neq(alice <=> eve);
    auto lt = std::is_lt(alice <=> eve);
    auto le = std::is_lteq(alice <=> eve);
    auto le = std::is_gt(alice <=> eve);
    auto le = std::is_gteq(alice <=> eve);
#endif  /* (__cplusplus > 201707L) */

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Non-member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - std::swap"s << '\n';
  {
    std::vector<int> alice{ 1, 2, 3, };
    std::vector<int> bob{ 7, 8, 9, 10, };

    auto print = [](const int & n) {
      std::cout << ' ' << n;
    };

    // Print state before swap
    std::cout << "alice:"s;
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << "\n" "bob  :"s;
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';

    std::cout << "-- SWAP\n"s;
    std::swap(alice, bob);

    // Print state after swap
    std::cout << "alice:"s;
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << "\n" "bob  :"s;
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  /// Non-member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector - std::erase, std::erase_if"s << '\n';
  {
    auto print_container = [](std::string_view comment,
                              std::vector<char> const & ctr) {
      std::cout << comment;
      for (auto xv : ctr) {
        std::cout << xv << ' ';
      }
      std::cout << '\n';
    };

    std::vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');
    print_container("Init:\n"s, cnt);

    std::erase(cnt, '3');
    print_container("Erase '3':\n"s, cnt);

#if (__cplusplus > 201707L)
    auto erased = std::erase_if(cnt, [](char x_) {
      return (x_ - '0') % 2 == 0;
    });

    print_container("Erase all even numbers:\n"s, cnt);
    std::cout << "In all "s << erased << " even numbers were erased.\n"s;
#else
    int erased = 0;
    std::erase_if(cnt, [&erased](char x_) {
      auto ok = (x_ - '0') % 2 == 0;
      if (ok) {
        ++erased;
      }
      return ok;
    });

    print_container("Erase all even numbers:\n"s, cnt);
    std::cout << "In all "s << erased << " even numbers were erased.\n"s;
#endif  /* (__cplusplus > 201707L) */

    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  return 0;
}

//  MARK: - C_vector_bool
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: C_vector_bool()
 *  std::vector<bool> is a possibly space-efficient specialization of
 *  std::vector for the type bool.
 *
 *  The manner in which std::vector<bool> is made space efficient (as well as whether
 *  it is optimized at all) is implementation defined. One potential optimization
 *  involves coalescing vector elements such that each element occupies a single bit
 *  instead of sizeof(bool) bytes.
 *
 *  std::vector<bool> behaves similarly to std::vector, but in order to be space
 *  efficient, it:
 *
 *  + Does not necessarily store its elements as a contiguous array.
 *
 *  + Exposes class std::vector<bool>::reference as a method of accessing individual
 *    bits. In particular, objects of this class are returned by operator[] by value.
 *
 *  + Does not use std::allocator_traits::construct to construct bit values.
 *
 *  + Does not guarantee that different elements in the same container can be modified
 *    concurrently by different threads.
 */
int C_vector_bool(int argc, const char * argv[]) {
  std::cout << "In "s << __func__ << std::endl;

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector<bool> - constructor, custom allocator"s << '\n';
  {
    auto constexpr cc_max(10ul);
    auto cc(0UL);
    auto prtboolean = [&cc](auto bl) {
      std::cout << std::boolalpha << std::setw(6) << bl << std::noboolalpha
                << (++cc % cc_max == 0 ? "\n"s : ""s);
    };

    std::vector<bool, valc::Mallocator<bool>> vbl;
    std::cout << "initial size: "s << std::setw(4) << vbl.size()
              << ", capacity: "s << std::setw(6) << vbl.capacity()
              << '\n';

    if (!vbl.empty()) {
      cc = 0ul;
      std::for_each(vbl.cbegin(), vbl.cend(), prtboolean);
      std::cout << '\n';
    }
    std::cout << "add a value.\n"s;
    vbl.push_back(true);
    std::cout << "add a value.\n"s;
    vbl.push_back(false);

    bool samples[] = {
      true, false, false, false, true,  true, false,  true, true, true,
      true,  true, false, true, false, false, false, false, true, true, };
    std::cout << "adding:\n"s;
    for (auto nr : samples) {
      std::cout << ' ' << std::boolalpha << nr;
      vbl.push_back(nr);
    }
    std::cout << std::noboolalpha << '\n';

    std::cout << "add a value.\n"s;
    vbl.push_back(true);
    std::cout << "add a value.\n"s;
    vbl.push_back(false);
    auto constexpr rn(100ul);
    std::cout << "reserve "s << rn << " more.\n"s;
    vbl.reserve(rn);

//      std::cout << vbl.data() << '\n';
    std::cout << "new size: "s << std::setw(4) << vbl.size()
              << ", capacity: "s << std::setw(6) << vbl.capacity()
              << '\n';
    cc = 0ul;
    if (!vbl.empty()) {
      std::for_each(vbl.cbegin(), vbl.cend(), prtboolean);
    }
    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector<bool> - get_allocator"s << '\n';
  {
    std::vector<bool> vb1(8);
    std::vector<bool, valc::Mallocator<bool>> vb2(8);
    std::vector<bool, valc::Mallocator<bool>> vb3(8);
    std::vector<uint8_t, valc::Mallocator<uint8_t>> vb4(8);

    [[maybe_unused]]
    auto vb1_al = vb1.get_allocator();
    auto vb2_al = vb2.get_allocator();
    auto vb3_al = vb3.get_allocator();
    auto vb4_al = vb4.get_allocator();

    std::cout << std::boolalpha;
    auto vb2_al_eq_vb2_al  = vb2_al == vb2_al;
    auto vb2_al_neq_vb2_al = vb2_al != vb2_al;
    std::cout << "vb2_al == vb2_al: "s << vb2_al_eq_vb2_al  << '\n';
    std::cout << "vb2_al != vb2_al: "s << vb2_al_neq_vb2_al << '\n';

    auto vb2_al_eq_vb3_al  = vb2_al == vb3_al;
    auto vb2_al_neq_vb3_al = vb2_al != vb3_al;
    std::cout << "vb2_al == vb3_al: "s << vb2_al_eq_vb3_al  << '\n';
    std::cout << "vb2_al != vb3_al: "s << vb2_al_neq_vb3_al << '\n';

    auto vb2_al_eq_vb4_al  = vb2_al == vb4_al;
    auto vb2_al_neq_vb4_al = vb2_al != vb4_al;
    std::cout << "vb2_al == vb4_al: "s << vb2_al_eq_vb4_al  << '\n';
    std::cout << "vb2_al != vb4_al: "s << vb2_al_neq_vb4_al << '\n';
    std::cout << std::noboolalpha;
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector<bool> - flip"s << '\n';
  {
    auto print = [](const std::vector<bool> & vb) {
      for (bool const b_ : vb) {
          std::cout << b_;
      }
      std::cout << '\n';
    };

    std::cout << std::noboolalpha;
    std::vector<bool> vbool { 0, 1, 0, 1, };
    print(vbool);
    vbool.flip();
    print(vbool);
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector<bool> - swap"s << '\n';
  {
    auto pv = [](auto el) {
      std::cout << std::setw(3) << el;
    };
    std::vector<bool> vb1{ 1, 0, };

    std::cout << std::noboolalpha;
    std::for_each(vb1.cbegin(), vb1.cend(), pv);
    std::cout << '\n';

    vb1.swap(vb1[0], vb1[1]);
    std::for_each(vb1.cbegin(), vb1.cend(), pv);
    std::cout << '\n';
  }
  std::cout << std::endl; //  make sure cout is flushed.

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::vector<bool> - std::hash"s << '\n';
  {
//    using vb = std::vector<bool>;

    auto to_vector_bool = [](unsigned nr) -> std::vector<bool> {
      std::vector<bool> vec;
      do {
          vec.push_back(nr & 1);
          nr >>= 1;
      } while (nr);

      return vec;
    };

    auto print = [](const std::vector<bool> & vec, bool new_line = true) {
      for (std::cout << "{ "s; bool const el : vec) {
        std::cout << el << ' ';
      }
      std::cout << '}' << (new_line ? '\n' : ' ');
    };

    for (auto i{0U}; i != 8; ++i) {
      std::cout << std::hex << std::uppercase;
      std::vector<bool> vec = to_vector_bool(i);
      std::cout << std::hash<std::vector<bool>>{}(vec) << ' ' << std::dec;
      print(vec);
    }

    // std::hash for vector<bool> makes it possible to keep them in
    // unordered_* associative containers, such as unordered_set.

    std::unordered_set vec {
      std::vector<bool> { 0 }, std::vector<bool> { 0, 0 }, std::vector<bool> { 1 },
      std::vector<bool> { 1 }, std::vector<bool> { 1, 0 }, std::vector<bool> { 1, 1 } };

    for (std::vector<bool> const & el : vec) {
      print(el, 0);
    }
  }
  std::cout << std::endl; //  make sure cout is flushed.

  return 0;
}
