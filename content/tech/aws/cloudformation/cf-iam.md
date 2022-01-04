---
title: "AWS CloudFormation での IAM 設定例"
url: "/p/pzihs8c"
date: "2021-04-05"
tags: ["AWS", "AWS/IAM"]
draft: true
---

{{< private >}}
- [AWS::IAM::Role (JP)](https://docs.aws.amazon.com/ja_jp/AWSCloudFormation/latest/UserGuide/aws-resource-iam-role.html)
{{< /private >}}


{{< code lang="yaml" >}}
AWSTemplateFormatVersion: "2010-09-09"
Resources:
  RootRole:
    Type: 'AWS::IAM::Role'
    Properties:
      AssumeRolePolicyDocument:
        Version: "2012-10-17"
        Statement:
          - Effect: Allow
            Principal:
              Service:
                - ec2.amazonaws.com
            Action:
              - 'sts:AssumeRole'
      Path: /
      Policies:
        - PolicyName: root
          PolicyDocument:
            Version: "2012-10-17"
            Statement:
              - Effect: Allow
                Action: '*'
                Resource: '*'
  RootInstanceProfile:
    Type: 'AWS::IAM::InstanceProfile'
    Properties:
      Path: /
      Roles:
        - !Ref RootRole
{{< /code >}}

