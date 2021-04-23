#include <eden.hpp>
#include <inductions.hpp>
#include <members.hpp>

namespace eden
{
   void eden::notify_lognewtempl(int32_t template_id,
                                 eosio::name authorized_creator,
                                 eosio::name collection_name,
                                 eosio::name schema_name,
                                 bool transferable,
                                 bool burnable,
                                 uint32_t max_supply,
                                 const atomicassets::attribute_map& immutable_data)
   {
      eosio::check(get_first_receiver() == atomic_assets_account, "Wrong atomicassets contract");
      eosio::check(authorized_creator == get_self(), "Wrong authorized creator");
      eosio::check(collection_name == ::eden::collection_name, "Wrong collection");
      eosio::check(schema_name == ::eden::schema_name, "Wrong schema");
      auto pos = std::find_if(immutable_data.begin(), immutable_data.end(),
                              [](const auto& attr) { return attr.key == "edenacc"; });
      eosio::check(pos != immutable_data.end(), "Missing account");
      eosio::name invitee(pos->value);
      inductions inductions{get_self()};
      auto induction = inductions.get_endorsed_induction(eosio::name(invitee));
      inductions.create_nfts(induction, template_id);

      members members{get_self()};
      members.set_nft(invitee, template_id);
   }

   void eden::notify_logmint(uint64_t asset_id,
                             eosio::name authorized_minter,
                             eosio::name collection_name,
                             eosio::name schema_name,
                             int32_t template_id,
                             eosio::name new_asset_owner,
                             const atomicassets::attribute_map& immutable_data,
                             const atomicassets::attribute_map& mutable_data,
                             std::vector<eosio::asset> tokens_to_back)
   {
      if (new_asset_owner != get_self())
         return;
      eosio::check(get_first_receiver() == atomic_assets_account, "Wrong atomicassets contract");
      eosio::check(authorized_minter == get_self(), "Wrong authorized creator");
      eosio::check(collection_name == ::eden::collection_name, "Wrong collection");
      eosio::check(schema_name == ::eden::schema_name, "Wrong schema");
      auto pos = std::find_if(immutable_data.begin(), immutable_data.end(),
                              [](const auto& attr) { return attr.key == "edenacc"; });
      eosio::check(pos != immutable_data.end(), "Missing account");
      eosio::name invitee(pos->value);
      inductions inductions{get_self()};
      auto induction = inductions.get_endorsed_induction(eosio::name(invitee));
      inductions.start_auction(induction, asset_id);
   }
}  // namespace eden