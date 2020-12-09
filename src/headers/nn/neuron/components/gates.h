import "../connections/connection.h"


template <class T=double>
class gate {
    
    public:
        void signal() {
            _con.signal()

        }

    protected:
        connection<T> _con;
        double _w = 1;
}