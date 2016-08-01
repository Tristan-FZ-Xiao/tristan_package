# update Makefile for OpenWrt Trunk r32885
# (c) 2012 Michael Fung 

include $(TOPDIR)/rules.mk

PKG_NAME:=proc_demo
PKG_VERSION:=0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
 SECTION:=kernel
 CATEGORY:=tristan
 TITLE:= proc demo
 MAINTAINER:= Tristan Xiao <tristan_xiao@qq.com>
endef

define Package/$(PKG_NAME)/description
    proc demo
endef

# note: must use TAB in this section:
define Package/$(PKG_NAME)/install        
 $(INSTALL_DIR) $(1)/etc
 $(INSTALL_DIR) $(1)/etc/init.d
 $(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/init.d/$(PKG_NAME)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
