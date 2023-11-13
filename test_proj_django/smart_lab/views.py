from django.shortcuts import render

# Create your views here.

from rest_framework import viewsets
from .serializers import ChemicalSerilizer, OperatorSerilizer
from .models import Chemical, Operator

from django.core.exceptions import PermissionDenied

class ChemicalViewSet(viewsets.ModelViewSet):
    serializer_class = ChemicalSerilizer
    queryset = Chemical.objects.all()

    def get_queryset(self):
        return self.queryset.all()
    
    def perform_create(self, serializer):
        serializer.save()

    def perform_update(self, serializer):
        serializer.save()
    
    def perform_destroy(self, instance):
        return super().perform_destroy(instance)

class OperatorViewSet(viewsets.ModelViewSet):
    serializer_class = OperatorSerilizer
    queryset = Operator.objects.all()

    def get_queryset(self):
        return self.queryset.all()
    
    def perform_create(self, serializer):
        serializer.save(operator_name = self.request.user)

