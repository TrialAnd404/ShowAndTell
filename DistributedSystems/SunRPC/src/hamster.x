typedef string nameString<32>;

struct hmstrState
{
	uint16_t treats_left;	/**< How many treats are left in stock? */
	uint32_t rounds;		/**< Number of hamster wheel revolutions */
	int16_t cost;			/**< expenses accumulated so far */
};

struct hmstr_new_in{
    nameString owner;
    nameString hamster;
    uint16_t treats;
};

struct hmstr_new_out{
    int32_t id;
};

struct hmstr_lookup_in{
    nameString owner;
    nameString hamster;
};

struct hmstr_lookup_out{
    int32_t id;
};

struct hmstr_directory_in{
    int32_t fdptr;
    nameString owner;
    nameString hamster;
};

struct hmstr_directory_out{
    int32_t id;
    int32_t fdptr;
};

struct hmstr_howsdoing_in{
    int32_t id;
};

struct hmstr_howsdoing_out{
    int32_t retVal;
	hmstrState state;	/**< [out] pointer to data structure where to store information */
};

struct hmstr_readentry_in{
    int32_t id;          
};

struct hmstr_readentry_out{
    int32_t treats;          
	nameString owner;   /**< [out] Where to store name of Owner    */
	nameString name;    /**< [out] Where to store name of Hamster  */
	int16_t price;      /**< [out] Where to store cost (up to now) */
};

struct hmstr_givetreats_in{
    int32_t id;
    uint16_t treats;
};

struct hmstr_givetreats_out{
    int32_t treats;
};

struct hmstr_collect_in{
    nameString owner;
};

struct hmstr_collect_out{
    int32_t price;
};

program HAMSTER_RPC {

    version HMSTR_COLLECT_VERS {
        hmstr_collect_out HMSTR_COLLECT_RPC(hmstr_collect_in) = 1;        
        hmstr_directory_out HMST_DIRECTORY_RPC(hmstr_directory_in) = 2;        
        hmstr_givetreats_out HMSTR_GIVETREATS_RPC(hmstr_givetreats_in) = 3;       
        hmstr_howsdoing_out HMSTR_HOWSDOING_RPC(hmstr_howsdoing_in) = 4;        
        hmstr_lookup_out HMSTR_LOOKUP_RPC(hmstr_lookup_in) = 5;        
        hmstr_readentry_out HMSTR_READENTRY_RPC(hmstr_readentry_in) = 6;        
        hmstr_new_out HMSTR_NEW_RPC(hmstr_new_in) = 7;
    } = 1;

} = 0x1BADBABE;