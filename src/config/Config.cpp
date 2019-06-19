#include "Config.h"
#include <getopt.h>
#include <stdexcept>
#include <cstring>
#include "UtilDefs.h"

namespace hx {

Config::Config() { 
}

Config::Config(int argc, char* argv[]) {
    const size_t opt_size = 7;
    static option long_options[opt_size] = {
        {"symbol",          required_argument, 0, 's'},
        {"side",            required_argument, 0, 'd'},
        {"max-qty",         required_argument, 0, 'q'},
        {"period",          required_argument, 0, 'p'},
        {"timeout",         required_argument, 0, 't'},
        {"mkt-endpoint",    required_argument, 0, 'm'},
        {"order-endpoint",  required_argument, 0, 'o'}
    };

    bool opt_seen[opt_size] = {false, false, false, false, false, false, false};
    int c = 0;
    while (c >= 0) {
        int opt_index = 0;
        if ((c = getopt_long_only(argc, argv, "s:d:q:p:t:m:o:", long_options, &opt_index)) < 0) {
            break;//end of options;
        }    
        if (opt_seen[opt_index]) {
            _DEATH("Option %s is provided twice!", optarg);
        }
        opt_seen[opt_index] = true;
        switch (c)
        {
        case 's': {  //SYMBOL
            //symbol_ = optarg;
            //if (symbol_.size() > SCXMsg::SYMBOL_CHAR_SIZE) {
            //    _DEATH("[Config] provided symbol is longer than 8 characters: %s", optarg);
            //}
            break;
        }
        case 'd': {  //SIDE
            //if (strcmp(optarg, "BUY") == 0) {
            break;
        }
        case 'q': { //MAX ORDER QTY
            //std::string max_qty_str(optarg);
            //max_order_qty_ = stringToUint32(max_qty_str);
            break;
        }
        case 'p': { //VWAP PERIOD (in seconds)
            break;
        }
        case 't': { //ORDER TIMEOUT (in seconds)
            break;
        }
        case 'm': { //MARKET DATA TCP IP AND PORT
            break;
        }
        case 'o': { //ORDER TCP IP AND PORT
            break;
        }
        default:
            _DEATH("[Config] invalid option %d", c);
        }
    }

    //If there are unkown options
    if (optind < argc) {
        while (optind < argc) {
            _DEATH("[Config] unkown argument %s", argv[optind++]);
        }
    }
    
    for (size_t i = 0; i < opt_size; ++i) {
        if (!opt_seen[i]) {
            _DEATH("[Config] missing argument '%s'!", long_options[i].name);
        }   
    }
};

void Config::print() const {
}

} //namespace scx
