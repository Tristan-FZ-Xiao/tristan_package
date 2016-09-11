# update Makefile for OpenWrt Trunk r32885
# (c) 2012 Michael Fung 

include $(TOPDIR)/rules.mk

PKG_NAME:=benchmark
PKG_VERSION:=0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
 SECTION:=basic
 CATEGORY:=tristan
 TITLE:= benchmark for SoC
 MAINTAINER:= Tristan Xiao <tristan_xiao@qq.com>
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp ./src/* $(PKG_BUILD_DIR) -af
endef

define Package/$(PKG_NAME)/description
 benchmark for SoC, supports C language and lua. Measure way: 1.basic operations; 2.cache.
endef

# note: must use TAB in this section:
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/sbin/$(PKG_NAME)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
