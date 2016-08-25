#include <sm/PropertyTree.hpp>
#include <sm/string_routines.hpp>
#include <sm/PropertyTreeImplementation.hpp>

namespace sm {
  
  PropertyTree::PropertyTree(boost::shared_ptr<PropertyTreeImplementation> imp, const std::string & baseNamespace) :
          _namespace(ensureTrailingBackslash(baseNamespace)), _imp(imp)
  {
    if(_namespace.size() > 0 && _namespace[0] != '/')
      _namespace = "/" + _namespace;
  }

  PropertyTree::PropertyTree(const PropertyTree & parent, const std::string & childNamespace) :
         _imp(parent._imp)
  {
    if(childNamespace.size() > 0 && childNamespace[0] == '/')
    {
      _namespace = ensureTrailingBackslash(childNamespace);
    }
    else
    {
      _namespace = ensureTrailingBackslash(parent._namespace + childNamespace);
    }

    if(_namespace.size() > 0 && _namespace[0] != '/')
      _namespace = "/" + _namespace;
  }

  PropertyTree::~PropertyTree()
  {
  }


  MutablePropertyTree::MutablePropertyTree(boost::shared_ptr<PropertyTreeImplementation> imp, const std::string & baseNamespace) :
    PropertyTree(imp, baseNamespace)
  {
  }


  MutablePropertyTree::MutablePropertyTree(const MutablePropertyTree & parent, const std::string & childNamespace) :
    PropertyTree(parent, childNamespace)
  {
  }


  MutablePropertyTree::~MutablePropertyTree()
  {
  }

  // \todo This function could do more name checking.
  std::string PropertyTree::buildQualifiedKeyName(const std::string & key) const
  {
    std::string qualifiedKeyName = !key.empty() && key[0] == '/' ? key : _namespace + key;
    if(qualifiedKeyName.size() > 1 && qualifiedKeyName.back() == '/'){
      qualifiedKeyName.resize(qualifiedKeyName.size() - 1);
    }
    return qualifiedKeyName;
  }

  double PropertyTree::getDouble(const std::string & key) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getDouble(buildQualifiedKeyName(key));
  }

  double PropertyTree::getDouble(const std::string & key, double defaultValue) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
      return boost::const_pointer_cast<const PropertyTreeImplementation>(_imp)->getDouble(buildQualifiedKeyName(key), defaultValue);
  }

  double MutablePropertyTree::getDouble(const std::string & key, double defaultValue)
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getDouble(buildQualifiedKeyName(key), defaultValue);
  }
  
  int PropertyTree::getInt(const std::string & key) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getInt(buildQualifiedKeyName(key));
  }

  int PropertyTree::getInt(const std::string & key, int defaultValue) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return boost::const_pointer_cast<const PropertyTreeImplementation>(_imp)->getInt(buildQualifiedKeyName(key),defaultValue);
  }

  int MutablePropertyTree::getInt(const std::string & key, int defaultValue)
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getInt(buildQualifiedKeyName(key),defaultValue);
  }

  bool PropertyTree::getBool(const std::string & key) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getBool(buildQualifiedKeyName(key));
  }

  bool PropertyTree::getBool(const std::string & key, bool defaultValue) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return boost::const_pointer_cast<const PropertyTreeImplementation>(_imp)->getBool(buildQualifiedKeyName(key), defaultValue);
  }

  bool MutablePropertyTree::getBool(const std::string & key, bool defaultValue) 
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getBool(buildQualifiedKeyName(key), defaultValue);
  }

  std::string PropertyTree::getString(const std::string & key) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getString(buildQualifiedKeyName(key));
  }

  std::string PropertyTree::getString(const std::string & key, const std::string & defaultValue) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return boost::const_pointer_cast<const PropertyTreeImplementation>(_imp)->getString(buildQualifiedKeyName(key), defaultValue);
  }

  std::string MutablePropertyTree::getString(const std::string & key, const std::string & defaultValue) 
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->getString(buildQualifiedKeyName(key), defaultValue);
  }
  
  bool PropertyTree::doesKeyExist(const std::string & key) const
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->doesKeyExist(buildQualifiedKeyName(key));
  }


  void MutablePropertyTree::setDouble(const std::string & key, double value)
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->setDouble(buildQualifiedKeyName(key), value);
  }

  void MutablePropertyTree::setInt(const std::string & key, int value)
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->setInt(buildQualifiedKeyName(key), value);
  }

  void MutablePropertyTree::setBool(const std::string & key, bool value)
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->setBool(buildQualifiedKeyName(key), value);
  }
  
  void MutablePropertyTree::setString(const std::string & key, const std::string & value)
  {
      SM_ASSERT_TRUE(Exception, _imp, "The implementation is NULL");
    return _imp->setString(buildQualifiedKeyName(key), value);
  }

  std::vector<ConstKeyPropertyTreePair> PropertyTree::getChildren() const {
    std::vector<ConstKeyPropertyTreePair> cchildren;
    auto children = _imp->getChildren(_namespace);
    cchildren.reserve(children.size());
    for (auto & kptp : children) {
      cchildren.push_back(ConstKeyPropertyTreePair{kptp.key, kptp.pt});
    }
    return cchildren;
  }

  std::vector<KeyPropertyTreePair> MutablePropertyTree::getChildren() const {
    return _imp->getChildren(_namespace);
  }
  
} // namespace sm
