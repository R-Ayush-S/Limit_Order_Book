#include<bits/stdc++.h>
using namespace std;

enum class ordertype {
	LIMIT,
	CANCEL,
	MARKET
};

enum class orderside {
	BUY,
	SELL
};

struct order {
	int id;
	int64_t price;
	double quantity;
	orderside side;
	time_t time;
	ordertype typ;
};

class OrderBook {
private:
map<int64_t,list<order>> sell;
map<int64_t,list<order>,greater<int64_t>> buy;
unordered_map<int,pair<int64_t,orderside>> cancel_map;
	
public:
	
void purge(){
//removing orders from list with quantity0;
	auto k=buy.begin();
	while(k!=buy.end()){
		auto it=k->second.begin();
		while(it!=k->second.end()){
		if(it->quantity==0){cancel_map.erase(it->id);it=k->second.erase(it);}
		else{it++;}
	}
	k++;
	}
	k=sell.begin();
	while(k!=sell.end()){
		auto it=k->second.begin();
		while(it!=k->second.end()){
		if(it->quantity==0){cancel_map.erase(it->id);it=k->second.erase(it);}
		else{it++;}
	}
	k++;
	}
}
	
void show_cancel(){
	auto it=cancel_map.begin();
	cout<<"--------->cancel_map order id's<----------\n";
	while(it!=cancel_map.end()){
		cout<<"---order id-->"<<it->first<<"<---------\n";
		it++;
	}
}

void show(){
	cout<<"\n\n";
	cout<<"\t\t\t<-BUY LIST->"<<endl;
	cout<<"Price_level\t\tOrder_id\t\tQuantity\n";
	auto k=buy.begin();
	while(k!=buy.end()){
		auto it=k->second.begin();
		while(it!=k->second.end()){
			cout<<k->first<<"\t\t"<<it->id<<"\t\t"<<it->quantity<<"\n";
			it++;
		}
		k++;
	}
	
	cout<<"\n\n";
	cout<<"\t\t\t<-SELL LIST->"<<endl;
	cout<<"Price_level\t\tOrder_id\t\tQuantity\n";
	k=sell.begin();
	while(k!=sell.end()){
		auto it=k->second.begin();
		while(it!=k->second.end()){
			cout<<k->first<<"\t\t"<<it->id<<"\t\t"<<it->quantity<<"\n";
			it++;
		}
		k++;
	}
	cout<<"\n\n";
}

void add_limit_order(order& ord_er){
	if(ord_er.quantity<=0||ord_er.price<=0){
		return;
	}
	else if(ord_er.side==orderside::BUY){
		buy[ord_er.price].push_back(ord_er);
	}
	else{
		sell[ord_er.price].push_back(ord_er);
	}
	if(ord_er.quantity>0&&ord_er.price>0)cancel_map[ord_er.id]={ord_er.price,ord_er.side};
}

void match(order& ord_er){
	if(ord_er.side==orderside::BUY){
		while(ord_er.quantity>0&&!sell.empty()&&ord_er.price>=sell.begin()->first){
		auto sell_list=sell.begin();
		list<order>& sell_pr=sell_list->second;
		while(sell_pr.size()>0){
			if(ord_er.quantity<=0){break;}
			else if(sell_pr.begin()->quantity>ord_er.quantity){
				sell_pr.begin()->quantity-=ord_er.quantity;
				ord_er.quantity=0;
			}
			else{
				ord_er.quantity-=(sell_pr.begin()->quantity);
				if(cancel_map.find(sell_pr.begin()->id)!=cancel_map.end()){cancel_map.erase(sell_pr.begin()->id);}
				sell_pr.erase(sell_pr.begin());
			}
		}
		
		if(sell_pr.empty()){
			sell_list=sell.erase(sell_list);
		}
		else{
			sell_list++;
		}
		}
	}
	else{
		while(ord_er.quantity>0&&!buy.empty()&&ord_er.price<=(buy.begin())->first){
		auto buy_list=buy.begin();
		list<order>& buy_pr=buy_list->second;
		while(buy_pr.size()>0){
			if(ord_er.quantity<=0){break;}
			else if(buy_pr.begin()->quantity>ord_er.quantity){
				buy_pr.begin()->quantity-=ord_er.quantity;
				ord_er.quantity=0;
			}
			else{
				ord_er.quantity-=(buy_pr.begin()->quantity);
				if(cancel_map.find(buy_pr.begin()->id)!=cancel_map.end()){cancel_map.erase(buy_pr.begin()->id);}
				buy_pr.erase(buy_pr.begin());
			}
		}
		
		if(buy_pr.empty()){
			buy_list=buy.erase(buy_list);
		}
		else{
			buy_list++;
		}
		}
	}
	if(ord_er.quantity==0){
		if(cancel_map.find(ord_er.id)!=cancel_map.end()){cancel_map.erase(ord_er.id);}
	}
	else{add_limit_order(ord_er);}
	
}

void cancel_order(int order_id){
	if(cancel_map.find(order_id)!=cancel_map.end()){
		auto pri=cancel_map[order_id].first;
		auto sid=cancel_map[order_id].second;
		if(sid==orderside::BUY){
			auto it=buy[pri].begin();
			while(it!=buy[pri].end()){
				if(it->id==order_id){
					it=buy[pri].erase(it);
					cancel_map.erase(order_id);
					if(buy[pri].empty()){buy.erase(pri);}
				}
				else{it++;}
			}
		}
		else{
			auto it=sell[pri].begin();
			while(it!=sell[pri].end()){
				if(it->id==order_id){
					it=sell[pri].erase(it);
					cancel_map.erase(order_id);
					if(sell[pri].empty()){sell.erase(pri);}
				}
				else{it++;}
			}
			
		}
	}
	else{return;}
}

void simulate_order_book(int num_orders) {
    srand(42);  // Seed for reproducibility

    int order_id = 1;
    int num_buys = 0;
    int num_sells = 0;
    int num_cancels = 0;
    int num_matches = 0;

    for (int i = 0; i < num_orders; ++i) {
        int action = rand() % 100;  // Random action: 0-100

        if (action < 40) {
            // 40% chance: Add a BUY order
            int64_t price = 10000 + rand()%1000;  // Price between 100.0 and 110.0
            double quantity = 1 + rand() % 100;           // Quantity between 1 and 100
            order buy_order = {order_id++, price, quantity, orderside::BUY, time(nullptr), ordertype::LIMIT};
            add_limit_order(buy_order);
            num_buys++;
        } else if (action < 80) {
            // 40% chance: Add a SELL order
            int64_t price = 10000 + rand()%1000;  // Price between 100.0 and 110.0
            double quantity = 1 + rand() % 100;           // Quantity between 1 and 100
            order sell_order = {order_id++, price, quantity, orderside::SELL, time(nullptr), ordertype::LIMIT};
            add_limit_order(sell_order);
            num_sells++;
        } else if (action < 90) {
            // 10% chance: Match a BUY order
            if (!sell.empty()) {
                int64_t price = 11000;  // High price to ensure matching
                double quantity = 1 + rand() % 50;
                order buy_order = {order_id++, price, quantity, orderside::BUY, time(nullptr), ordertype::MARKET};
                match(buy_order);
                num_matches++;
            }
        } else if (action < 95) {
            // 5% chance: Match a SELL order
            if (!buy.empty()) {
                int64_t price = 10000;  // Low price to ensure matching
                double quantity = 1 + rand() % 50;
                order sell_order = {order_id++, price, quantity, orderside::SELL, time(nullptr), ordertype::MARKET};
                match(sell_order);
                num_matches++;
            }
        }else {
            // 5% chance: Cancel a random order
            if (!cancel_map.empty()) {
                auto it = cancel_map.begin();
                advance(it, rand() % cancel_map.size());
                int id_to_cancel = it->first;
                cancel_order(id_to_cancel);
                num_cancels++;
            }
		}
    }


    cout << "\n========== Simulation Results ==========\n";
    cout << "Total Orders Processed: " << num_orders << "\n";
    cout << "BUY Orders Added: " << num_buys << "\n";
    cout << "SELL Orders Added: " << num_sells << "\n";
    cout << "Matches Performed: " << num_matches << "\n";
    cout << "Cancellations: " << num_cancels << "\n";
    cout << "=========================================\n";
}

void validate_order_book() {
    cout << "\n--- Validating Order Book ---\n";

    // Check BUY book integrity
    for (const auto& [price, orders] : buy) {
        for (const auto& ord : orders) {
            assert(cancel_map.find(ord.id) != cancel_map.end() && "Order missing from cancel_map");
            assert(cancel_map[ord.id].first == price && "Price mismatch in cancel_map");
            assert(cancel_map[ord.id].second == orderside::BUY && "Side mismatch in cancel_map");
        }
    }

    // Check SELL book integrity
    for (const auto& [price, orders] : sell) {
        for (const auto& ord : orders) {
            assert(cancel_map.find(ord.id) != cancel_map.end() && "Order missing from cancel_map");
            assert(cancel_map[ord.id].first == price && "Price mismatch in cancel_map");
            assert(cancel_map[ord.id].second == orderside::SELL && "Side mismatch in cancel_map");
        }
    }

    cout << "Validation Passed!\n";
}

};

int main(){
	auto start_time=chrono::high_resolution_clock::now();
	OrderBook ob;
	ob.simulate_order_book(100000);
	ob.validate_order_book();
	auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	cout<<"total time taken(in ms)="<<duration.count();
	return 0;
}