#include "coding_schemes/abe.hpp"

void abe::encoder(std::istream &in, std::ostream &out) {
    // mask
    uint32_t mask = (1<<bitwidth)-1;
    
    // caches
    uint32_t window_cache[window_size];
    uint32_t window_transitions[window_size-1];
    uint32_t savings_cache[bitwidth];

    while( in.peek() != EOF ) {
        // clusters and savings
        std::vector<int> clusters[bitwidth];
        float savings[bitwidth];

        // load in window cache
        for(int w=0;w<window_size;w++) {
            if( in.peek() == EOF ) {
                break;
            }
            // read into window cache
            in >> window_cache[w];
        }

        // get window transitions
        for(int w=0;w<window_size-1;w++) {
            // get transitions
            window_transitions[w] = window_cache[w]^window_cache[w+1];
        }

        // iterate over each busline
        for(int i=0;i<bitwidth;i++) {
            int combined_transitions[bitwidth];
            savings[i] = 0;
            for(int j=0;j<bitwidth;j++) {
                // initialise combined transitions
                combined_transitions[j] = 0;
                // skip identical line
                if (i==j) {
                    continue;
                }
                // get transtions per bus line
                for(int w=0;w<window_size-1;w++) {
                    combined_transitions[j] += ((window_transitions[w]>>i)&1)*(2*((window_transitions[w]>>j)&1)-((window_transitions[w]>>i)&1));
                }
                // add clusters and savings
                if(combined_transitions[j] > 0) {
                    clusters[i].push_back(j);
                    savings[i] += combined_transitions[j];
                }
                
            }
        }

        // find argmax of clusters
        int basis = 0;
        for(int i=0;i<bitwidth;i++) {
            if(savings[i] > savings[basis]) {
                basis = i;
            }
        }
        
        // apply cluster to output
        for (int cluster : clusters[basis]) {
            for(int w=0;w<window_size;w++) {
                window_cache[w] ^= ((window_cache[w]>>basis)&1) << cluster;
            }
        }
        
        // send to stdout
        for(int w=0;w<window_size;w++) {
            out << (window_cache[w]|(1<<bitwidth)) << std::endl; 
        }
        
        // add end of frame
        out << (1<<basis) << std::endl;
    }
}

abe::abe(unsigned int bitwidth, unsigned int window_size) : coding_scheme(bitwidth), window_size(window_size){}
