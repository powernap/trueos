/*

This is the default Jenkins Pipeline config used to test pull-requests
on GitHub trueos/trueos repo.

*/

pipeline {
  agent { label 'TrueOS-PR' }

  environment {
    GH_ORG = 'trueos'
    GH_REPO = 'trueos'
  }

  stages {
    stage('Queued') {
        agent {
        label 'JenkinsMaster'
      }
      steps {
        echo "Build queued"
      }
    }

    stage('Checkout') {
      steps {
        checkout scm
      }
    }

    stage('Pre-Clean') {
      steps {
        sh 'make clean'
        sh 'cd release && make clean'
      }
    }
    stage('World') {
      steps {
        sh 'make -j32 buildworld'
      }
    }
    stage('Kernel') {
      steps {
        sh 'make -j32 buildkernel'
      }
    }
    stage('Base Packages') {
      environment {
           PKGSIGNKEY = credentials('a50f9ddd-1460-4951-a304-ddbf6f2f7990')
      }
      steps {
        sh 'make -j32 packages'
      }
    }
    stage('Ports') {
      environment {
           PKGSIGNKEY = credentials('a50f9ddd-1460-4951-a304-ddbf6f2f7990')
      }
      steps {
        sh 'cd release && make poudriere'
      }
    }
    stage('Release') {
      post {
        success {
          archiveArtifacts artifacts: 'artifacts/**', fingerprint: true
        }
      }
      steps {
        sh 'rm -rf ${WORKSPACE}/artifacts'
        sh 'cd release && make release'
        sh 'mkdir -p ${WORKSPACE}/artifacts/repo'
        sh 'cp /usr/obj${WORKSPACE}/amd64.amd64/release/*.iso ${WORKSPACE}/artifacts'
        sh 'cp /usr/obj${WORKSPACE}/amd64.amd64/release/*.txz ${WORKSPACE}/artifacts'
        sh 'cp /usr/obj${WORKSPACE}/amd64.amd64/release/MANIFEST ${WORKSPACE}/artifacts'
        sh 'cp -r /usr/obj${WORKSPACE}/repo/* ${WORKSPACE}/artifacts/repo/'
      }
    }
  }
  post {
    always {
      echo "*** Cleaning up ***"
      sh 'make clean >/dev/null 2>/dev/null'
      sh 'cd release && make clean >/dev/null 2>/dev/null '
      sh 'rm -rf /usr/obj${WORKSPACE}'
      script {
        cleanWs notFailBuild: true
      }
    }
  }
}
