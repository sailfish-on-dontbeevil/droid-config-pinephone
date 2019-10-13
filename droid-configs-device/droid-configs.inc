# This file should be %%included into a device specific spec file
# where macros are defined:
#
# community_adaptation: needs to be defined for community HW adaptations
#
# Device information:
# device: should be the CM codename or the AOSP TARGET_PRODUCT
# vendor: determine the droid-side directory used for ./device/<vendor>/<device>
# device_pretty: User-visible model name of the device
# vendor_pretty: User-visible manufacturer name of the device
# rpm_device: device name used rpm-side (eg in configs) : defaults to device
# rpm_vendor: vendor name used rpm-side (eg in configs) : defaults to vendor
# ofono_enable_plugins: any additional ofono plugins that you want explicitly enabled
# ofono_disable_plugins: any ofono plugins that you want explicitly disabled

# Device capabilities:
# remove_modem: set this if your device has no modem (e.g. a WiFi-only tablet)
# remove_bluetooth: set this if your device does not have a Bluetooth chip
# pixel_ratio: UI scaling factor, choose from 1.0, 1.5, 2.0. Any other value will
# default the icon pack to 1.0 resolution (it is a bug)!
# Read more on ways to do-it-together: https://bugs.nemomobile.org/show_bug.cgi?id=814
# WARNING: pixel_ratio is experimental, and not yet ready for proper UI scalability
# tests! Please do not test/port your apps to other form factors for the time being.
#  Ratio examples:
#  Jolla1:       1.0
#  Jolla Tablet: 1.5
#  Nexus 5:      2.0
#  Nexus 4:      pixel_ratio=1.3 (icon resolution then drops to 1.0)

# Overwriting other configs:
# If your droid-config-$DEVICE needs to provide one or more device-specific
# configuration files, do the following:
# * find the package of those generic configs (e.g. rpm -qf /etc/ofono/ril_subscription.conf)
# * copy ALL files, e.g.:
#   cd $ANDROID_ROOT/hybris/droid-configs/sparse/; rpm2cpio .../ofono-configs-mer...rpm | cpio -idv
# * make needed modifications for your device
# * add "Provides: PACKAGE" to your droid-config-$DEVICE.spec (e.g. Provides: ofono-configs)


# We assume all devices have a cellular modem, unless explicitly removed:
%if 0%{!?remove_modem:1}
%define have_modem 1
%endif

# We assume all devices have Bluetooth, unless explicitly removed:
%if 0%{!?remove_bluetooth:1}
%define have_bluetooth 1
%endif

# dcd_path is defined for hysterical reasons (coming from dhd builds on OBS)
# now obsolete, hence always the default:
%define dcd_path .

# dcd_common is the common stuff and dcd_sparse is the common sparse
%define dcd_common %{dcd_path}/droid-configs-device
%define dcd_sparse droid-configs-device/sparse

# Set defaults if not defined already:
%if 0%{!?rpm_device:1}
%define rpm_device %{device}
%endif
%if 0%{!?rpm_vendor:1}
%define rpm_vendor %{vendor}
%endif

%define board_mapping_dir %{_datadir}/ssu/board-mappings.d
%define board_mapping_file %{board_mapping_dir}/05-%{rpm_vendor}-%{rpm_device}.ini

Name:	    droid-config-%{rpm_device}
Provides:   droid-config
Summary:    Config packages for Droid HAL adaptations
Version:    1
Release:    1
Group:      Configs
License:    GPLv2
Source0:    %{name}-%{version}.tar.bz2
%if 0%{!?native_build:1}
BuildRequires: ssu-kickstart-configuration
BuildRequires: pkgconfig(android-headers)
%endif
BuildRequires: repomd-pattern-builder
BuildRequires: qt5-qttools-kmap2qmap
%if 0%{?community_adaptation:1}
# because it provides one ssu feature, do:
Requires: community-adaptation
# because it has a template for dhc to provide a ssu feature, do:
BuildRequires: community-adaptation
%endif
%if 0%{?have_bluetooth:1}
Provides: bluetooth-rfkill-event-configs
# Device-specific obexd configuration common to BlueZ 4 & 5
Requires: obexd-calldata-provider
Requires: obexd-contentfilter-helper
Provides: obexd-configs

# Require a BlueZ configuration
Requires: %{rpm_device}-bluez-configs
%endif
# The pc_suite usb mode (implies obex) can conflict with bluez5
Conflicts: usb-moded-pc-suite-mode-android
Obsoletes: usb-moded-pc-suite-mode-android
# Config packages from usb-moded source with overlapping purposes
Conflicts: usb-moded-mtp-mode-android-ffs
Obsoletes: usb-moded-mtp-mode-android-ffs
%description
%{summary}.

%if 0%{!?native_build:1}
%package    ssu-kickstarts
Provides:   droid-config-ssu-kickstarts
Summary:    Kickstarts for Droid HAL
Provides:   image-configurations

%description ssu-kickstarts
%{summary}.
%endif

################################################################
%package patterns
Summary:    Repository patterns for %{rpm_device} hw
Provides:   droid-config-patterns
Provides:   package-groups

%description patterns
%{summary}.

################################################################
%package policy-settings
Summary:    Policy settings for %{rpm_device} hw
Provides:   droid-config-policy-settings
Requires:   ohm >= 1.1.16
Requires:   ohm-plugins-misc >= 1.2.0
Requires:   ohm-plugins-dbus
Requires:   ohm-plugin-telephony
Requires:   ohm-plugin-signaling
Requires:   ohm-plugin-media
Requires:   ohm-plugin-accessories
Requires:   ohm-plugin-resolver
Requires:   ohm-plugin-ruleengine
Requires:   ohm-plugin-profile
Requires:   ohm-plugin-route
Requires:   pulseaudio-modules-nemo-common >= 11.1.24
Requires:   pulseaudio-policy-enforcement >= 11.1.35
Requires:   policy-settings-common >= 0.7.3
Obsoletes:  ohm-config <= 1.1.15
# ohm-configs-default should not be installed ever, thus no version
# specification defined here.
Obsoletes:  ohm-configs-default
Provides:   ohm-configs > 1.1.15
Provides:   policy-settings

%description policy-settings
%{summary}.

%if 0%{!?native_build:1}
################################################################
%package kickstart-configuration
Summary:    Kickstart configuration for %{rpm_device}
Provides:   droid-config-kickstart-configuration
Requires:   ssu-kickstart-configuration
Provides:   droid-hal-kickstart-configuration
# the %{name} contains ssu.ini file which is needed to build kickstarts
Requires:   %{name} = %{version}-%{release}

%description kickstart-configuration
%{summary}.
%endif

################################################################
%package preinit-plugin
Summary:    Preinit plugins for %{rpm_device}
Provides:   droid-config-preinit-plugins

%description preinit-plugin
%{summary}.

################################################################
%package pulseaudio-settings
Summary:    PulseAudio settings for %{rpm_device} hw
Provides:   droid-config-pulseaudio-settings
Requires:   pulseaudio >= 11.1+git4
Requires:   pulseaudio-modules-nemo-parameters >= 11.1.24
Requires:   pulseaudio-modules-nemo-stream-restore >= 11.1.24
Requires:   pulseaudio-modules-nemo-mainvolume >= 11.1.24
Requires:   pulseaudio-modules-droid >= 11.1.66
Requires:   pulseaudio-policy-enforcement >= 11.1.35
Provides:   pulseaudio-settings

%description pulseaudio-settings
%{summary}.

################################################################
%package sailfish
Provides: droid-config-sailfish
Provides: theme-configs
Group:	System
BuildArch:	noarch
Requires: %{name} = %{version}-%{release}
Requires: oneshot
Requires: dconf
Summary: Per device configuration for sailfish for droid-hal device: %{rpm_device}

%description sailfish
Configure sailfish eg naturally landscape devices like mako

################################################################

%package flashing
Provides: droid-config-flashing
Group: System
# Sparse image flashing support for flash-partition
Requires: droid-hal-tools
Summary: On device flashing files for %{rpm_device} hw

%description flashing
%{summary}.

################################################################

%if 0%{?out_of_image_files:1}

%package out-of-image-files
Group:	System
BuildArch: noarch
Summary: Files that are used for flashing and are not needed on device.

%description out-of-image-files
Contains files that are used for flashing but are not needed inside image, e.g.,
flashing configurations or flashing scripts.

%endif

################################################################

%if 0%{?have_bluetooth:1}
%package bluez4
Summary: %{rpm_device} packages for BlueZ 4
Conflicts: droid-config-%{rpm_device}-bluez5
Requires: %{name} = %{version}-%{release}
Requires: %{name} = %{version}
Provides: %{rpm_device}-bluez-configs

Requires: bluez
Conflicts: bluez5

Requires: bluez-libs
Conflicts: bluez5-libs

Requires: obexd
Conflicts: bluez5-obexd

Requires: obexd-server
# no obexd-server equivalent in BlueZ 5, so no conflict

Requires: kf5bluezqt-bluez4
Conflicts: kf5bluezqt-bluez5

Provides: bluez-configs
Conflicts: bluez5-configs
Obsoletes: bluez-configs-sailfish
Obsoletes: bluez-configs-mer

%description bluez4
%{summary}.

%package bluez5
Summary: %{rpm_device} packages for BlueZ 5
Conflicts: droid-config-%{rpm_device}-bluez4
Requires: %{name} = %{version}-%{release}
Requires: %{name} = %{version}
Provides: %{rpm_device}-bluez-configs

Requires: bluez5
Conflicts: bluez

Requires: bluez5-libs
Conflicts: bluez-libs

Requires: bluez5-obexd
Conflicts: obexd

Requires: kf5bluezqt-bluez5
Conflicts: kf5bluezqt-bluez4

Provides: bluez5-configs
Conflicts: bluez-configs
Obsoletes: bluez-configs-sailfish

%description bluez5
%{summary}.
%endif

################################################################

%prep
%if 0%{?_obs_build_project:1}
# For OBS builds we need to have tarball extracted after tar_git packaging it
%setup -q -n %{name}-%{version}
%endif

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}

# Amalgamate configs files from device-specific and all- trees
# Retain permissions:
rm -rf tmp/
mkdir -p tmp/
echo "%defattr(-,root,root,-)" > tmp/droid-config.files

# Prefer files from sparse/ in the HA specific
# area over sparse/ in the dcd area
copy_files_from() {
  config_dir=$1
  if [ -d $config_dir ]; then
    (cd $config_dir; find . \( -type f -o -type l \) -print ) | sed 's/^.//' >> tmp/droid-config.files
    cp -Rf $config_dir/* $RPM_BUILD_ROOT/
  fi
}

delete_files() {
  files=$1
  deletelist=$2
  dorm=$3
  if [ -e $deletelist ]; then
    egrep -v '^#|^$' $deletelist | (
      while read file; do
	[ "x$dorm" == "x1" ] && rm $RPM_BUILD_ROOT/$file
	grep -vE "$file" $files > tmp/$$.files
	mv tmp/$$.files $files
      done)
  fi
}

config_files() {
  files=$1
  configlist=$2
  if [ -e $configlist ]; then
    egrep -v '^#|^$' $configlist | (
      while read file; do
        escape=$(echo "$file" | sed 's,/,\\\/,g')
        sed -e "s/$escape/%config $escape/g" $files > tmp/$$.files
        mv tmp/$$.files $files
      done)
  fi
}

# Copy from common; erase any we don't want; overlay from device
# specific sparse/ :
copy_files_from %{dcd_path}/%{dcd_sparse}
delete_files tmp/droid-config.files delete_file.list 1
copy_files_from %{dcd_path}/sparse
delete_files tmp/droid-config.files delete_file_%{rpm_device}.list 1
# This add %config to %files section for files from rpm-config-files.files
config_files tmp/droid-config.files rpm-config-files.files

OFONONOPLUGIN=$RPM_BUILD_ROOT/var/lib/environment/ofono/noplugin.conf

%if 0%{?have_modem:1}
# DO NOT MODIFY THE "OFONO_MODEM_DEFAULT_PLUGINS" SET! If you do, you will break
# modems on other devices! If you need to introduce or remove a plugin,
# use ofono_enable/disable_plugins macro in your device(s) .spec file.
OFONO_MODEM_DEFAULT_PLUGINS=ril,rildev,rilmodem,sms_history,push_forwarder,\
push_notification,smart_messaging,mnclength,provision,nettime,connman,atmodem

echo $OFONO_MODEM_DEFAULT_PLUGINS | sed s/,/\\n/g | (
  while read line; do
    sed -i '/,'$line'\\/d' $OFONONOPLUGIN
  done)
%endif

%if 0%{?ofono_enable_plugins:1}
echo %{ofono_enable_plugins} | sed s/,/\\n/g | (
  while read line; do
    sed -i '/,'$line'\\/d' $OFONONOPLUGIN
  done)
%endif

%if 0%{?ofono_disable_plugins:1}
echo %{ofono_disable_plugins} | sed s/,/\\n/g | (
  while read line; do
    echo ','$line'\' >> $OFONONOPLUGIN
  done)
%endif

# Delete files from droid-config.files which are packaged in main spec file
if [ -e packaged-in-main-spec.files ]; then
  delete_files tmp/droid-config.files packaged-in-main-spec.files 0
fi

# We want to keep some files in separate subpackages.
# NOTE: some files might get to wrong place with this because of string assumption.
%if 0%{?have_bluetooth:1}
grep bluez4 tmp/droid-config.files > tmp/bluez4.files || true
sed --in-place '/bluez4/d' tmp/droid-config.files
grep bluez5 tmp/droid-config.files > tmp/bluez5.files || true
sed --in-place '/bluez5/d' tmp/droid-config.files
%endif
echo "%defattr(-,root,root,-)" > tmp/policy-settings.files
grep ohm tmp/droid-config.files > tmp/policy-settings.files
sed --in-place '/ohm/d' tmp/droid-config.files
echo "%defattr(-,root,root,-)" > tmp/pulseaudio-settings.files
grep pulse tmp/droid-config.files > tmp/pulseaudio-settings.files
sed --in-place '/pulse/d' tmp/droid-config.files
sed --in-place '/preinit/d' tmp/droid-config.files
grep "/dconf/db/" tmp/droid-config.files > tmp/sailfish-settings.files
sed --in-place '/\/dconf\/db\//d' tmp/droid-config.files
grep -e "flash-partition" -e "platform-updates" tmp/droid-config.files > tmp/flashing.files
sed --in-place -e '/flash-partition/d' -e '/platform-updates/d' tmp/droid-config.files

%if 0%{?out_of_image_files:1}
if [ -e out-of-image-files.files ]; then
  delete_files tmp/droid-config.files out-of-image-files.files 0
  cp out-of-image-files.files tmp/out-of-image-files.files
fi
%endif

# Now the majority of the sparse tree is made we can handle configs
# which need some kind of substitution or generating

# patterns
mkdir -p %{buildroot}/usr/share/package-groups/
/usr/bin/repomd-pattern-builder.py --patternxml -p %{dcd_common}/patterns/common -o %{buildroot}/usr/share/package-groups/ --version=%{version} --release=%{release}
/usr/bin/repomd-pattern-builder.py --patternxml -p %{dcd_path}/patterns/ -o %{buildroot}/usr/share/package-groups/ --version=%{version} --release=%{release}

delete_patterns() {
  deletelist=$1
  if [ -e $deletelist ]; then
    egrep -v '^#|^$' $deletelist | (
      while read file; do
        rm $RPM_BUILD_ROOT/$file
      done)
  fi
}

delete_patterns delete_pattern_%{rpm_device}.list

%if 0%{!?pixel_ratio:1}
%define pixel_ratio 1.0
%endif

sed --in-place 's|@PIXEL_RATIO@|%{pixel_ratio}|' %{buildroot}/etc/dconf/db/vendor.d/silica-configs.txt

%define start_drag_distance %(awk 'BEGIN{print int(%{pixel_ratio}*20)}')
sed --in-place 's|@START_DRAG_DISTANCE@|%{start_drag_distance}|' %{buildroot}/etc/xdg/QtProject/QPlatformTheme.conf

# icon_res can be only one of 1.0, 1.25, 1.5, 1.75 or 2.0 use pixel_ratio and pick closest one
%define icon_res %(awk 'BEGIN {a=int((%{pixel_ratio}-0.125)/0.25)*0.25+0.25;a=(a<=1?"1.0":(a>=2.0?"2.0":a));print a }')

sed --in-place 's|@ICON_RES@|%{icon_res}|' %{buildroot}/etc/dconf/db/vendor.d/silica-configs.txt
sed --in-place 's|@ICON_RES@|%{icon_res}|' %{buildroot}/usr/share/package-groups/*

# In localbuilds it will break the system, because droid-hal-version-$DEVICE is
# not built yet. Also it will pull many unnecessary things into the SDK target.
%if 0%{?_obs_build_project:1}
sed --in-place 's|@DEVICE@|%{rpm_device}|' %{buildroot}/etc/zypp/systemCheck.d/*.check
%else
sed --in-place '/systemCheck.d/d' tmp/droid-config.files
rm -f %{buildroot}/etc/zypp/systemCheck.d/*.check
%endif

# SSU board mapping for hardware adaptation
%if 0%{!?provides_own_board_mapping:1}
mkdir -p $RPM_BUILD_ROOT/%{board_mapping_dir}
sed -e 's|%DEVICE%|%{rpm_device}|g' \
    -e 's|%VENDOR%|%{rpm_vendor}|g' \
    -e 's|%OTHERDEVICE%|%{otherdevice}|g' \
    -e "s|%DEVICE_PRETTY%|%{device_pretty}|g" \
    -e "s|%VENDOR_PRETTY%|%{vendor_pretty}|g" \
    -e "s|%ARCH%|%{_target_cpu}|g" \
    droid-configs-device/ssu/board-mappings.d/05-sfe.ini.in >$RPM_BUILD_ROOT/%{board_mapping_file}
%endif

%if 0%{?community_adaptation:1}
# Community ports have their adaptation repos on http://build.merproject.org
# Until we figure out how ssu works, we'll prevent the default adaptation0 from being created...
sed --in-place '/adaptation-repos=/d' $RPM_BUILD_ROOT/%{board_mapping_dir}/*.ini
# ...and make a separate adaptation repo for community:
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/ssu/features.d/
sed -e 's|@DEVICE@|%{rpm_device}|g' \
    -e 's|@VENDOR@|%{rpm_vendor}|g' \
    %{_datadir}/community-adaptation/adaptation-community.ini.in \
    >$RPM_BUILD_ROOT/%{_datadir}/ssu/features.d/adaptation-community.ini
%endif

%if 0%{!?native_build:1}
# Copy kickstart packs (for %%{rpm_device}-kickstart-configuration)
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/ssu/kickstart/pack/%{rpm_device}
DEST_UPDATER=$RPM_BUILD_ROOT/%{_datadir}/ssu/kickstart/pack/%{rpm_device}/hybris
sed -e 's|@DEVICE@|%{rpm_device}|g' %{dcd_common}/kickstart/pack_package-droid-updater > $DEST_UPDATER || true
if [ -f %{dcd_path}/kickstart/pack_package-droid-updater ]; then
  sed -e 's|@DEVICE@|%{rpm_device}|g' %{dcd_path}/kickstart/pack_package-droid-updater > $DEST_UPDATER || true
fi

# and attachments (override if there is one)
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/ssu/kickstart/attachment
cp -f %{dcd_common}/kickstart/attachment_hybris $RPM_BUILD_ROOT/%{_datadir}/ssu/kickstart/attachment/%{rpm_device}
cp -f %{dcd_path}/kickstart/attachment_hybris $RPM_BUILD_ROOT/%{_datadir}/ssu/kickstart/attachment/%{rpm_device} || true
%endif

# Override the common configs with device specifics if those exist
if [ -e %{dcd_path}/kickstart ]; then
  cp -rf %{dcd_path}/kickstart/* $RPM_BUILD_ROOT/%{_datadir}/ssu/kickstart/
fi

# if we are building on Mer OBS or locally, make a domain=sales .ks
# so it works for HADK users:
%define disturl_subst %(echo %{disturl} | grep jollamobile.com)
%if "%{disturl_subst}" == ""
  echo "Setting domain to sales"
  %define ssu_override domain=sales
%else
  echo "Setting domain to jolla"
  %define ssu_override domain=jolla
%endif

# build rnd kickstarts on devel level, release kickstarts on all other levels
%if 0%{?qa_stage_devel:1}
  KS_LEVELS=true
%else
  KS_LEVELS=false
%endif

%if 0%{!?native_build:1}
# build variant ks files if name file exist
%if 0%{?variant_name_list:1}
  variants=variant_name.list
  if [ -e $variants ]; then
    egrep -v '^#|^$' $variants | (
      while read line; do
        %gen_ks "$line"
        sed -i s/@VARIANT_NAME@/$line/g %{buildroot}/%{_datadir}/kickstarts/*$line*.ks
      done)
  fi
%else
  %gen_ks %{rpm_device}
%endif
%endif

# Preinit plugins
ln -s /bin/false $RPM_BUILD_ROOT%{_libdir}/startup/preinit/is_total_erase_supported

# Create keyboard map file
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/qt5/keymaps
/usr/lib/qt5/bin/kmap2qmap %{dcd_common}/configs/droid.kmap %{dcd_common}/configs/us.kmap $RPM_BUILD_ROOT/%{_datadir}/qt5/keymaps/droid.qmap
/usr/lib/qt5/bin/kmap2qmap %{dcd_path}/configs/droid.kmap %{dcd_path}/configs/us.kmap $RPM_BUILD_ROOT/%{_datadir}/qt5/keymaps/droid.qmap || true

clean_files() { sed 's_^./_/_'; }

%if 0%{!?native_build:1}
# Create the various '%%files' files
(cd $RPM_BUILD_ROOT; ls .%{_datadir}/kickstarts/*.ks) | clean_files > tmp/ssu-kickstarts.files

(cd $RPM_BUILD_ROOT; find .%{_datadir}/ssu/kickstart/ -type f) | clean_files > tmp/kickstart-configuration.files
(cd $RPM_BUILD_ROOT; find .%{_datadir}/ssu/kickstart/ -type l) | clean_files >> tmp/kickstart-configuration.files
%endif

# Append extra_files to the end of droid-config.files.
if [ -e extra-files.files ]; then
  cat extra-files.files >> tmp/droid-config.files
fi

# In some cases we might need to create empty directories that are e.g. mount points
if [ -e create.dirs ]; then
  for A in $(cat create.dirs); do
    mkdir -p $RPM_BUILD_ROOT/$A
  done
fi

################################################################
%post
# Force an update of ssu repositories
touch %{board_mapping_dir}/* || :
touch %{_datadir}/ssu/features.d/* || :

# To add additional post scripts, one can define those as part
# of additional_post_scripts macro.
%{?additional_post_scripts}

################################################################
%post sailfish
%{_bindir}/add-oneshot dconf-update || :

################################################################
%files -f tmp/droid-config.files
%defattr(-,root,root,-)
%if 0%{!?provides_own_board_mapping:1}
%{board_mapping_file}
%endif
%{_datadir}/qt5/keymaps/droid.qmap
%if 0%{?community_adaptation:1}
%{_datadir}/ssu/features.d/adaptation-community.ini
%endif

%if 0%{!?native_build:1}
%files ssu-kickstarts
%defattr(-,root,root,-)
%{_datadir}/kickstarts/*.ks
%endif

%files patterns
%defattr(-,root,root,-)
%{_datadir}/package-groups/*.xml

%files policy-settings -f tmp/policy-settings.files
%defattr(644,root,root,-)

%if 0%{!?native_build:1}
%files kickstart-configuration -f tmp/kickstart-configuration.files
%defattr(644,root,root-)
%endif

%files preinit-plugin
%defattr(-,root,root,-)
%{_libdir}/startup/preinit/*

%files pulseaudio-settings -f tmp/pulseaudio-settings.files
%defattr(644,root,root,-)
 
%files sailfish -f tmp/sailfish-settings.files
%defattr(-,root,root,-)

%files flashing -f tmp/flashing.files
%defattr(-,root,root,-)

%if 0%{?out_of_image_files:1}
%files out-of-image-files -f tmp/out-of-image-files.files
%endif

%if 0%{?have_bluetooth:1}
%posttrans bluez4
rm -rf /etc/bluetooth                   || :
ln -s bluez4/bluetooth /etc/bluetooth   || :

%posttrans bluez5
rm -rf /etc/bluetooth                   || :
ln -s bluez5/bluetooth /etc/bluetooth   || :

%files bluez4 -f tmp/bluez4.files
%defattr(644,root,root,-)

%files bluez5 -f tmp/bluez5.files
%defattr(644,root,root,-)
%endif
