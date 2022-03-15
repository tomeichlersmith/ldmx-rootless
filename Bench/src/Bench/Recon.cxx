
#include "Bench/Event/Hit.h"

#ifdef USE_ROOT_FRAMEWORK
#include "Framework/EventProcessor.h"
#else
#include "fire/Processor.h"
#endif

#include <random>

namespace bench {

#ifdef USE_ROOT_FRAMEWORK
namespace fire = framework;
#endif

class Recon : public fire::Producer {
  /// the random number generator, unseeded so it produces the same results each time
  std::mt19937 rng;
  /// the distribution of sizes
  std::uniform_int_distribution<std::size_t> rand_index;
 public:
#ifdef USE_ROOT_FRAMEWORK
  Recon(const std::string& name, framework::Process& p)
    : framework::Producer(name,p),
#else
  Recon(const fire::config::Parameters& ps)
    : fire::Producer(ps),
#endif
    rng{}, // this is where a seed for the RNG would be put
    rand_index{0, 99}
  {}
  ~Recon() = default;
  void produce(fire::Event& event) final override {
#ifdef USE_ROOT_FRAMEWORK
    const auto& rand_data = event.getCollection<Hit>("randdata");
#else
    const auto& rand_data = event.get<std::vector<Hit>>("randdata");
#endif
    std::size_t i;
    do {
      i = rand_index(rng);
    } while(i >= rand_data.size());
    
    Hit special = rand_data.at(i);
    event.add("specialhit", special);
  }
};

}

#ifdef USE_ROOT_FRAMEWORK
DECLARE_PRODUCER_NS(bench,Recon);
#else
DECLARE_PROCESSOR_NS(bench,Recon);
#endif
