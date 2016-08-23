# update Makefile for OpenWrt Trunk r32885
# (c) 2012 Michael Fung 

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=proc_demo
PKG_VERSION:=0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

# the FILES is MUST for Kernel module
define KernelPackage/$(PKG_NAME)
 SECTION:= tristan
 CATEGORY:=tristan
 TITLE:= proc demo
 FILES:=$(PKG_BUILD_DIR)/*.$(LINUX_KMOD_SUFFIX)
 MAINTAINER:= Tristan Xiao <tristan_xiao@qq.com>
endef

define Package/$(PKG_NAME)/description
  provide an example for how to use add a proc entry for setting up communication between Application and Kernel.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp -af ./src/* $(PKG_BUILD_DIR)/
endef

EXTRA_KCONFIG:= \
	CONFIG_PROC_DEMO=m

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	ARCH="$(LINUX_KARCH)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="-g $(BUILDFLAGS)" \
	$(EXTRA_KCONFIG) \
	modules
endef

# note: must use TAB in this section:
#define KernelPackage/$(PKG_NAME)/install
#	$(INSTALL_DIR) $(1)/etc
#	$(INSTALL_DIR) $(1)/etc/init.d
#	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/init.d/$(PKG_NAME)
#endef

$(eval $(call KernelPackage,$(PKG_NAME)))
