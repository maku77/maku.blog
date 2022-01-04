---
title: "Amazon EC2 をコマンドライン (CLI) で操作する"
url: "/p/zkx9ju6"
date: "2021-02-22"
tags: ["AWS"]
working: true
---

{{< private >}}
{{< code >}}
Linux または macOS

// Read from a file in the current directory
$ aws ec2 describe-instances --filters file://filter.json

// Read from a file in /tmp
$ aws ec2 describe-instances --filters file:///tmp/filter.json
Windows

// Read from a file in C:\temp
C:\> aws ec2 describe-instances --filters file://C:\temp\filter.json
{{< /code >}}
{{< /private >}}


EC2 で使用可能なリージョンの一覧を取得する (ec2 describe-regions)
----

{{< code >}}
$ aws ec2 describe-regions
{{< /code >}}

{{< accordion title="実行結果 (YAML形式）" >}}
{{< code lang="yaml" >}}
Regions:
- Endpoint: ec2.eu-north-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: eu-north-1
- Endpoint: ec2.ap-south-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: ap-south-1
- Endpoint: ec2.eu-west-3.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: eu-west-3
- Endpoint: ec2.eu-west-2.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: eu-west-2
- Endpoint: ec2.eu-west-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: eu-west-1
- Endpoint: ec2.ap-northeast-3.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: ap-northeast-3
- Endpoint: ec2.ap-northeast-2.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: ap-northeast-2
- Endpoint: ec2.ap-northeast-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: ap-northeast-1
- Endpoint: ec2.sa-east-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: sa-east-1
- Endpoint: ec2.ca-central-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: ca-central-1
- Endpoint: ec2.ap-southeast-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: ap-southeast-1
- Endpoint: ec2.ap-southeast-2.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: ap-southeast-2
- Endpoint: ec2.eu-central-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: eu-central-1
- Endpoint: ec2.us-east-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: us-east-1
- Endpoint: ec2.us-east-2.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: us-east-2
- Endpoint: ec2.us-west-1.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: us-west-1
- Endpoint: ec2.us-west-2.amazonaws.com
  OptInStatus: opt-in-not-required
  RegionName: us-west-2
{{< /code >}}
{{< /accordion >}}


EC2 インスタンスの情報を取得する (ec2 describe-instances)
----

作成済みの EC2 インスタンスの一覧、あるいは指定した EC2 インスタンスの情報を取得します。

{{< code >}}
$ aws ec2 describe-instances
{{< /code >}}

{{< accordion title="実行結果（YAML形式）" >}}
{{< code lang="yaml" >}}
Reservations:
- Groups: []
  Instances:
  - AmiLaunchIndex: 0
    Architecture: x86_64
    BlockDeviceMappings:
    - DeviceName: /dev/xvda
      Ebs:
        AttachTime: '2021-03-02T14:26:39+00:00'
        DeleteOnTermination: true
        Status: attached
        VolumeId: vol-3b75f0f9090990420
    CapacityReservationSpecification:
      CapacityReservationPreference: open
    ClientToken: ''
    CpuOptions:
      CoreCount: 1
      ThreadsPerCore: 1
    EbsOptimized: false
    EnaSupport: true
    EnclaveOptions:
      Enabled: false
    HibernationOptions:
      Configured: false
    Hypervisor: xen
    ImageId: ami-e2e9e713809d28faa
    InstanceId: i-757968a71071691b7
    InstanceType: t2.nano
    KeyName: ec2key
    LaunchTime: '2021-03-02T15:32:17+00:00'
    MetadataOptions:
      HttpEndpoint: enabled
      HttpPutResponseHopLimit: 1
      HttpTokens: optional
      State: applied
    Monitoring:
      State: disabled
    NetworkInterfaces:
    - Attachment:
        AttachTime: '2021-03-02T14:26:38+00:00'
        AttachmentId: eni-attach-c1c18b4c61083b1ea
        DeleteOnTermination: true
        DeviceIndex: 0
        NetworkCardIndex: 0
        Status: attached
      Description: ''
      Groups:
      - GroupId: sg-650f7192f60271eb7
        GroupName: launch-wizard-1
      InterfaceType: interface
      Ipv6Addresses: []
      MacAddress: 0a:a0:bf:37:7f:c1
      NetworkInterfaceId: eni-c8aea6d54ac8cc0d0
      OwnerId: '049344049957'
      PrivateDnsName: ip-172-31-0-95.ap-northeast-1.compute.internal
      PrivateIpAddress: 172.31.0.95
      PrivateIpAddresses:
      - Primary: true
        PrivateDnsName: ip-172-31-0-95.ap-northeast-1.compute.internal
        PrivateIpAddress: 172.31.0.95
      SourceDestCheck: true
      Status: in-use
      SubnetId: subnet-13e65081
      VpcId: vpc-af6e084d
    Placement:
      AvailabilityZone: ap-northeast-1c
      GroupName: ''
      Tenancy: default
    PrivateDnsName: ip-172-31-0-95.ap-northeast-1.compute.internal
    PrivateIpAddress: 172.31.0.95
    ProductCodes: []
    PublicDnsName: ''
    RootDeviceName: /dev/xvda
    RootDeviceType: ebs
    SecurityGroups:
    - GroupId: sg-650f7192f60271eb7
      GroupName: launch-wizard-1
    SourceDestCheck: true
    State:
      Code: 80
      Name: stopped
    StateReason:
      Code: Client.UserInitiatedShutdown
      Message: 'Client.UserInitiatedShutdown: User initiated shutdown'
    StateTransitionReason: User initiated (2021-03-03 16:24:03 GMT)
    SubnetId: subnet-6508113e
    Tags:
    - Key: Name
      Value: maku-test-ec2
    VirtualizationType: hvm
    VpcId: vpc-af6e084d
  OwnerId: '493440499570'
  ReservationId: r-09d469440a8ee4861
{{< /code >}}
{{< /accordion >}}


AMI の情報を取得する (ec2 describe-images)
----

{{< code >}}
$ aws ec2 describe-images
{{< /code >}}

作成した Amazon マシンイメージ (AMI) の一覧を表示することができます。

{{< code title="例: 最新の 5 つの AMI を新しい順に表示" >}}
$ aws ec2 describe-images \
  --owners self \
  --query 'reverse(sort_by(Images,&CreationDate))[:5].{id:ImageId,date:CreationDate}'
{{< /code >}}

{{< accordion title="実行結果（JSON形式）" >}}
{{< code lang="json" >}}
[
    {
        "id": "ami-0a1b2c3d4e5f60001",
        "date": "2018-11-28T17:16:38.000Z"
    },
    {
        "id": "ami-0a1b2c3d4e5f60002",
        "date": "2018-09-15T13:51:22.000Z"
    },
    {
        "id": "ami-0a1b2c3d4e5f60003",
        "date": "2018-08-19T10:22:45.000Z"
    },
    {
        "id": "ami-0a1b2c3d4e5f60004",
        "date": "2018-05-03T12:04:02.000Z"
    },
    {
        "id": "ami-0a1b2c3d4e5f60005",
        "date": "2017-12-13T17:16:38.000Z"
    }
]
{{< /code >}}
{{< /accordion >}}


アタッチされた EBS ボリュームの情報を取得する (ec2 describe-volumes)
----

{{< code >}}
$ aws ec2 describe-volumes
{{< /code >}}

EC2 インスタンスにアタッチされた Elastic Block Store (EBS) ボリュームの情報を調べることができます。

{{< accordion title="実行結果" >}}
{{< code lang="json" >}}
{
    "Volumes": [
        {
            "AvailabilityZone": "us-west-2a",
            "Attachments": [
                {
                    "AttachTime": "2013-09-17T00:55:03.000Z",
                    "InstanceId": "i-a071c394",
                    "VolumeId": "vol-e11a5288",
                    "State": "attached",
                    "DeleteOnTermination": true,
                    "Device": "/dev/sda1"
                }
            ],
            "VolumeType": "standard",
            "VolumeId": "vol-e11a5288",
            "State": "in-use",
            "SnapshotId": "snap-f23ec1c8",
            "CreateTime": "2013-09-17T00:55:03.000Z",
            "Size": 30
        },
        {
            "AvailabilityZone": "us-west-2a",
            "Attachments": [
                {
                    "AttachTime": "2013-09-18T20:26:16.000Z",
                    "InstanceId": "i-4b41a37c",
                    "VolumeId": "vol-2e410a47",
                    "State": "attached",
                    "DeleteOnTermination": true,
                    "Device": "/dev/sda1"
                }
            ],
            "VolumeType": "standard",
            "VolumeId": "vol-2e410a47",
            "State": "in-use",
            "SnapshotId": "snap-708e8348",
            "CreateTime": "2013-09-18T20:26:15.000Z",
            "Size": 8
        }
    ]
}
{{< /code >}}
{{< /accordion >}}


仮想マシンを終了する (ec2 terminate-instances)
----

{{< code >}}
$ aws ec2 terminate-instances --instance-ids "$INSTANCE_ID"
{{< /code >}}


仮想マシンが起動・終了するまで待機 (ec2 wait)
----

### 起動するまで待機

`ec2 run-instances` で仮想マシンを起動したあと、起動完了まで待機したいときは次のようにします。

{{< code >}}
$ aws ec2 wait instance-running --instance-ids "$INSTANCE_ID"
{{< /code >}}

### 終了するまで待機

`ec2 terminate-instances` で仮想マシンを終了したあと、終了完了まで待機したいときは次のようにします。

{{< code >}}
$ aws ec2 wait instance-terminated --instance-ids "$INSTANCE_ID"
{{< /code >}}


VPC の情報を取得する (ec2 describe-vpcs)
----

{{< code title="全 VPC の情報を取得する" >}}
$ aws ec2 describe-vpcs
{{< /code >}}

{{< accordion title="実行結果（YAML形式）" >}}
{{< code lang="yaml" >}}
Vpcs:
- CidrBlock: 172.31.0.0/16
  CidrBlockAssociationSet:
  - AssociationId: vpc-cidr-assoc-3d305bd3
    CidrBlock: 172.31.0.0/16
    CidrBlockState:
      State: associated
  DhcpOptionsId: dopt-6fa8cf64
  InstanceTenancy: default
  IsDefault: true
  OwnerId: '049344049957'
  State: available
  VpcId: vpc-af6e084d
{{< /code >}}
{{< /accordion >}}

{{< code title="VPC の ID だけ取得する" >}}
$ aws ec2 describe-vpcs --query "Vpcs[0].VpcId" --output text
vpc-af6e084d
{{< /code >}}


サブネットの情報を取得する (ec2 describe-subnets)
----

{{< code >}}
$ aws ec2 describe-subnets
{{< /code >}}

{{< accordion title="実行結果（YAML形式）" >}}
{{< code lang="yaml" >}}
Subnets:
- AssignIpv6AddressOnCreation: false
  AvailabilityZone: ap-northeast-1a
  AvailabilityZoneId: apne1-az4
  AvailableIpAddressCount: 4091
  CidrBlock: 172.31.32.0/20
  DefaultForAz: true
  Ipv6CidrBlockAssociationSet: []
  MapCustomerOwnedIpOnLaunch: false
  MapPublicIpOnLaunch: true
  OwnerId: '049344049957'
  State: available
  SubnetArn: arn:aws:ec2:ap-northeast-1:049344049957:subnet/subnet-cd6e2662
  SubnetId: subnet-cd6e2662
  VpcId: vpc-af6e084d
- AssignIpv6AddressOnCreation: false
  AvailabilityZone: ap-northeast-1d
  AvailabilityZoneId: apne1-az2
  AvailableIpAddressCount: 4091
  CidrBlock: 172.31.16.0/20
  DefaultForAz: true
  Ipv6CidrBlockAssociationSet: []
  MapCustomerOwnedIpOnLaunch: false
  MapPublicIpOnLaunch: true
  OwnerId: '049344049957'
  State: available
  SubnetArn: arn:aws:ec2:ap-northeast-1:049344049957:subnet/subnet-3d7d568b
  SubnetId: subnet-3d7d568b
  VpcId: vpc-af6e084d
- AssignIpv6AddressOnCreation: false
  AvailabilityZone: ap-northeast-1c
  AvailabilityZoneId: apne1-az1
  AvailableIpAddressCount: 4090
  CidrBlock: 172.31.0.0/20
  DefaultForAz: true
  Ipv6CidrBlockAssociationSet: []
  MapCustomerOwnedIpOnLaunch: false
  MapPublicIpOnLaunch: true
  OwnerId: '049344049957'
  State: available
  SubnetArn: arn:aws:ec2:ap-northeast-1:049344049957:subnet/subnet-113e6508
  SubnetId: subnet-113e6508
  VpcId: vpc-af6e084d
{{< /code >}}
{{< /accordion >}}

