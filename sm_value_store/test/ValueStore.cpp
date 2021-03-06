#include <gtest/gtest.h>
#include <sm/BoostPropertyTree.hpp>
#include <sm/value_store/LayeredValueStore.hpp>
#include <sm/value_store/PropertyTreeValueStore.hpp>

TEST(ValueStoreSuite, testSimplePropertyTreeValueStore)
{
  sm::BoostPropertyTree pt;

  pt.setDouble("d", 0.1);
  pt.setDouble("d/i", 10);
  pt.setDouble("d/d", 0.2);
  pt.setDouble("d/d/d", 0.3);
  pt.setDouble("d/d/j", 11);
  EXPECT_EQ(0.1, pt.getDouble("d"));
  EXPECT_EQ(10, pt.getInt("d/i"));
  EXPECT_EQ(0.2, pt.getDouble("d/d"));
  EXPECT_EQ(0.3, pt.getDouble("d/d/d"));
  EXPECT_EQ(11, pt.getInt("d/d/j"));
  EXPECT_EQ(11, pt.getInt("d/d/j"));

  sm::ExtendibleValueStoreRef vpt(pt);

  EXPECT_FALSE(vpt.hasKey("BLA"));
  EXPECT_TRUE(vpt.hasKey("d"));
  EXPECT_EQ(0.1, vpt.getDouble("d").get());
  EXPECT_TRUE(vpt.hasKey("d/d"));
  EXPECT_EQ(0.2, vpt.getDouble("d/d").get());
  auto dChild = vpt.getValueStore().getChild("d");
  EXPECT_EQ("d", dChild.getKey());
  EXPECT_EQ(0.1, dChild.getValueStore().getDouble("").get());
  EXPECT_EQ(0.2, dChild.getValueStore().getDouble("d").get());

  auto dChildF = vpt.getChild("d");
  EXPECT_EQ(0.2, dChildF.getDouble("d").get());
  auto ddChildF = dChildF.getChild("d");
  EXPECT_EQ(0.3, ddChildF.getDouble("d").get());

  {
    const std::vector<const char *> expectedKeys{"i", "d"};
    int i = 0;
    for (auto & c : dChildF.getChildren()){
      ASSERT_LT(i, expectedKeys.size());
      EXPECT_EQ(expectedKeys[i++], c.getKey());
    }
    EXPECT_EQ(expectedKeys.size(), i);
  }
  {
    const std::vector<const char *> expectedKeys{"d", "j"};
    int i = 0;
    for (auto & c : ddChildF.getChildren()){
      ASSERT_LT(i, expectedKeys.size());
      EXPECT_EQ(expectedKeys[i++], c.getKey());
    }
    EXPECT_EQ(expectedKeys.size(), i);
  }
  {
    const std::vector<const char *> expectedKeys{"d"};
    int i = 0;
    for (auto & c : vpt.getChildren()){
      ASSERT_LT(i, expectedKeys.size());
      EXPECT_EQ(expectedKeys[i++], c.getKey());
      if(c.getKey() == "d"){
        EXPECT_NEAR(0.1, c.getValueStore().getDouble(""), 1e-16);
        EXPECT_NEAR(0.2, c.getValueStore().getDouble("d"), 1e-16);
        const std::vector<const char *> expectedKeys = { "i", "d"};
        int i = 0;
        for (auto & c2 : c.getChildren()){
          ASSERT_LT(i, expectedKeys.size());
          EXPECT_EQ(expectedKeys[i++], c2.getKey());
        }
        EXPECT_EQ(expectedKeys.size(), i);
      }
    }
    EXPECT_EQ(expectedKeys.size(), i);
  }

  vpt.addInt("ai", 4);
  EXPECT_EQ(4, pt.getInt("ai"));
  EXPECT_EQ(4, vpt.getInt("ai").get());

  // TODO (HannesSommer) complete ExtendibleValueStore (testing)
  //  auto nDchild = vpt.addChild("nD");
  //  nDchild.addInt("i", 3);
}

TEST(ValueStoreSuite, convertValueStoreToPropertyTree)
{
  sm::BoostPropertyTree pt;

  pt.setDouble("d", 0.1);
  pt.setDouble("d/i", 10);
  pt.setDouble("d/d", 0.2);
  pt.setDouble("d/d/d", 0.3);
  pt.setDouble("d/d/j", 11);
  pt.setString("d/s", "BLA");
  sm::ExtendibleValueStoreRef vpt(pt);

  sm::ConstPropertyTree ptvpt = vpt.asPropertyTree();

  EXPECT_EQ(0.1, ptvpt.getDouble("d"));
  EXPECT_EQ(10, ptvpt.getInt("d/i"));
  EXPECT_EQ(0.2, ptvpt.getDouble("d/d"));
  EXPECT_EQ(0.3, ptvpt.getDouble("d/d/d"));
  EXPECT_EQ("BLA", ptvpt.getString("d/s"));

  auto child = vpt.getChild("d");
  EXPECT_EQ(0.1, child.getDouble(""));
  EXPECT_EQ(10, child.getInt("i"));
  EXPECT_EQ(0.2, child.getDouble("d"));
  EXPECT_EQ(0.3, child.getDouble("d/d"));
  EXPECT_EQ("BLA", child.getString("s").get());

  sm::ConstPropertyTree ptvptc = child.asPropertyTree();
  EXPECT_EQ(0.1, ptvptc.getDouble(""));
  EXPECT_EQ(10, ptvptc.getInt("i"));
  EXPECT_EQ(0.2, ptvptc.getDouble("d"));
  EXPECT_EQ(0.3, ptvptc.getDouble("d/d"));
  EXPECT_EQ("BLA", ptvptc.getString("s"));
}

TEST(ValueStoreSuite, layeredValueStore)
{
  sm::BoostPropertyTree pt;
  pt.setDouble("d", 0.1);
  sm::BoostPropertyTree pt2;
  pt2.setDouble("a", 0.1);
  sm::value_store::ValueStoreRef vpt(pt);
  sm::value_store::ValueStoreRef vpt2(pt2);

  sm::value_store::ValueStoreRef lpt(std::shared_ptr<sm::value_store::LayeredValueStore>(new sm::value_store::LayeredValueStore{vpt.getValueStoreSharedPtr(), vpt2.getValueStoreSharedPtr()}));

  EXPECT_FALSE(lpt.hasKey("BLA"));
  EXPECT_TRUE(lpt.hasKey("d"));
  EXPECT_EQ(0.1, lpt.getDouble("d"));
  EXPECT_TRUE(lpt.hasKey("a"));
  EXPECT_EQ(0.1, lpt.getDouble("a"));
}
